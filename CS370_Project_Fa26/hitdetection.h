
//houseproject refs
void render_scene();
void render_hud();

vec4 index_to_color(int i) {
    return vec4(
        ((i & 0xFF)      ) / 255.0f,
        ((i >> 8) & 0xFF ) / 255.0f,
        ((i >> 16) & 0xFF) / 255.0f,
        1.0f
    );
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

    glDeleteBuffers(1, &pickColorBuf);

    // Read the pixel (y is already bottom-origin from your mouse code)
    unsigned char pixel[4];
    glReadPixels((GLint)mx, (GLint)my, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);

    int index = color_to_index(pixel[0], pixel[1], pixel[2]);
    // IMPORTANT: immediately redraw the real scene so the next swap is correct
    // (this prevents the colored ID pass from appearing on screen)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    render_scene();
    render_hud();
    glFlush();

    if (index > 0 && index <= (int)objects.size())
        return objects[index - 1];
    return nullptr;
}