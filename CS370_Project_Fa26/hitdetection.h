
// Encode object index into a unique color (supports up to ~16M objects)
vec4 index_to_color(int i) {
    return vec4(
        ((i & 0xFF)       ) / 255.0f,
        ((i >> 8) & 0xFF  ) / 255.0f,
        ((i >> 16) & 0xFF ) / 255.0f,
        1.0f
    );
}

int color_to_index(unsigned char r, unsigned char g, unsigned char b) {
    return r | (g << 8) | (b << 16);
}

// Returns pointer to the clicked SceneObject, or nullptr
SceneObject* pick_scene_object(float mx, float my) {
    // Save current state (matrices are already set by display, but we re-set them to be safe)
    mat4 saved_proj   = proj_matrix;
    mat4 saved_camera = camera_matrix;
    mat4 saved_model  = model_matrix;

    // Use the same projection/camera that the scene uses
    // (they are already computed in display(); if you call this from the callback you may need to recompute them)

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (size_t i = 0; i < objects.size(); ++i) {
        SceneObject* obj = objects[i];
        // Build model matrix exactly as SceneObject::draw() does
        mat4 trans   = translate(obj->position);
        mat4 rotX    = vmath::rotate(obj->rotation[0], vec3(1,0,0));
        mat4 rotY    = vmath::rotate(obj->rotation[1], vec3(0,1,0));
        mat4 rotZ    = vmath::rotate(obj->rotation[2], vec3(0,0,1));
        mat4 scaleM  = vmath::scale(obj->scale);
        model_matrix = trans * rotX * rotY * rotZ * scaleM;

        // Temporarily force a solid color that encodes the index
        // Easiest: reuse an existing solid-color path.
        // If you have a per-object colorBuffer you can overwrite it,
        // or just create a temporary color buffer once and update it.
        // Quick version using the color shader + a one-off buffer:
        // (you can cache one color buffer and glBufferSubData into it)
        GLuint tempColorBuf;
        glGenBuffers(1, &tempColorBuf);
        vec4 col = index_to_color((int)i);
        // ... build a solid color buffer of the right vertex count with 'col'
        // then:
        draw_color_object(obj->mesh, tempColorBuf);   // or your existing color draw
        glDeleteBuffers(1, &tempColorBuf);
    }

    // Read the pixel (OpenGL y is bottom-origin; your mouse code already flipped it)
    unsigned char pixel[4];
    glReadPixels((GLint)mx, (GLint)my, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);

    int index = color_to_index(pixel[0], pixel[1], pixel[2]);

    // Restore matrices (the next display() will clear and redraw normally anyway)
    proj_matrix   = saved_proj;
    camera_matrix = saved_camera;
    model_matrix  = saved_model;

    if (index >= 0 && index < (int)objects.size())
        return objects[index];
    return nullptr;
}