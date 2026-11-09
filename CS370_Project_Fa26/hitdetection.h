//houseproject refs
void render_scene();
void render_hud();
enum GizmoAxis {GIZMO_NONE = 0, GIZMO_X = 1001, GIZMO_Y = 1002, GIZMO_Z = 1003 };
GizmoAxis activeGizmo = GIZMO_NONE;
int rawIndex = -1;


vec4 index_to_color(int i) {
    return vec4(
        ((i & 0xFF)      ) / 255.0f,
        ((i >> 8) & 0xFF ) / 255.0f,
        ((i >> 16) & 0xFF) / 255.0f,
        1.0f
    );
}

int get_raw_index() {
    return rawIndex;
}

int color_to_index(unsigned char r, unsigned char g, unsigned char b) {
    return r | (g << 8) | (b << 16);
}

// Helper that draws one object with a raw color buffer ID (not an index into ColorBuffers)
void draw_picking_object(GLuint mesh, GLuint colorBuffer) {
    glUseProgram(color_program);
    glUniformMatrix4fv(color_proj_mat_loc, 1, GL_FALSE, proj_matrix);
    glUniformMatrix4fv(color_cam_mat_loc, 1, GL_FALSE, camera_matrix);
    glUniformMatrix4fv(color_model_mat_loc, 1, GL_FALSE, model_matrix);

    glBindVertexArray(VAOs[mesh]);

    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[mesh][PosBuffer]);
    glVertexAttribPointer(color_vPos, posCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(color_vPos);

    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glVertexAttribPointer(color_vCol, colCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(color_vCol);

    glDrawArrays(GL_TRIANGLES, 0, numVertices[mesh]);
}


//function returns clicked scene object
SceneObject* pick_scene_object(float mx, float my) {
    // Make sure we have the same projection/camera the scene used
    // (recompute them here so the callback doesn't depend on leftover state)
    GLfloat xratio = 1.0f, yratio = 1.0f;
    if (ww <= hh) yratio = (GLfloat)hh / (GLfloat)ww;
    else if (hh <= ww) xratio = (GLfloat)ww / (GLfloat)hh;

    proj_matrix = ortho(-radius*xratio, radius*xratio, -radius*yratio, radius*yratio, -100.0, 100.0);
    camera_matrix = lookat(eye, center, up);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // One reusable color buffer for the whole pick pass
    GLuint pickColorBuf;
    glGenBuffers(1, &pickColorBuf);

    for (size_t i = 0; i < objects.size(); ++i) {
        SceneObject* obj = objects[i];

        if (obj->mesh == (GLuint)-1)
            continue;

        // Build model matrix the same way SceneObject::draw does
        mat4 trans  = translate(obj->position);
        mat4 rotX   = vmath::rotate(obj->rotation[0], vec3(1,0,0));
        mat4 rotY   = vmath::rotate(obj->rotation[1], vec3(0,1,0));
        mat4 rotZ   = vmath::rotate(obj->rotation[2], vec3(0,0,1));
        mat4 scaleM = vmath::scale(obj->scale);
        model_matrix = trans * rotX * rotY * rotZ * scaleM;

        // Fill the color buffer with the index color
        vec4 col = index_to_color((int)i + 1);
        vector<vec4> colors(numVertices[obj->mesh], col);
        glBindBuffer(GL_ARRAY_BUFFER, pickColorBuf);
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(vec4) * numVertices[obj->mesh],
                     colors.data(),
                     GL_DYNAMIC_DRAW);

        draw_picking_object(obj->mesh, pickColorBuf);
    }



    if (selectedObject && gizmoArrowVAO != 0) {
        glDisable(GL_DEPTH_TEST);

        float size = 0.75f;
        float length = 1.0f;
        float offset = length * 2.0f;

        mat4 trans = translate(selectedObject->position);
        mat4 trans_offset = translate(0.0f, offset, 0.0f);
        mat4 scale = vmath::scale(size, length, size);

        auto drawPickArrow = [&](const mat4& model, int id) {
            model_matrix = model;
            vec4 col = index_to_color(id);

            vector<vec4> colors(gizmoArrowNumVerts, col);
            glBindBuffer(GL_ARRAY_BUFFER, pickColorBuf);
            glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(vec4), colors.data(), GL_DYNAMIC_DRAW);

            glUseProgram(color_program);
            glUniformMatrix4fv(color_proj_mat_loc,  1, GL_FALSE, proj_matrix);
            glUniformMatrix4fv(color_cam_mat_loc,   1, GL_FALSE, camera_matrix);
            glUniformMatrix4fv(color_model_mat_loc, 1, GL_FALSE, model_matrix);

            glBindVertexArray(gizmoArrowVAO);
            glBindBuffer(GL_ARRAY_BUFFER, gizmoArrowPosBuffer);
            glVertexAttribPointer(color_vPos, posCoords, GL_FLOAT, GL_FALSE, 0, NULL);
            glEnableVertexAttribArray(color_vPos);

            glBindBuffer(GL_ARRAY_BUFFER, pickColorBuf);
            glVertexAttribPointer(color_vCol, colCoords, GL_FLOAT, GL_FALSE, 0, NULL);
            glEnableVertexAttribArray(color_vCol);

            glDrawArrays(GL_TRIANGLES, 0, gizmoArrowNumVerts);
        };

        // X
        mat4 rotate = vmath::rotate(90.0f, vec3(0,0,1));
        drawPickArrow(trans * rotate * trans_offset * scale, GIZMO_X);

        // Y
        drawPickArrow(trans * trans_offset * scale, GIZMO_Y);

        // Z
        rotate = vmath::rotate(-90.0f, vec3(1,0,0));
        drawPickArrow(trans * rotate * trans_offset * scale, GIZMO_Z);

        glEnable(GL_DEPTH_TEST);
    }

    // Read the pixel (y is already bottom-origin from your mouse code)
    unsigned char pixel[4];
    glReadPixels((GLint)mx, (GLint)my, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);

    int index = color_to_index(pixel[0], pixel[1], pixel[2]);
    rawIndex = index;

    glDeleteBuffers(1, &pickColorBuf);

    // IMPORTANT: immediately redraw the real scene so the next swap is correct
    // (this prevents the colored ID pass from appearing on screen)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);
    render_scene();
    render_hud();
    glFlush();

    if (index > 0 && index <= (int)objects.size())
        return objects[index - 1];
    return nullptr;
}