void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    //press escape either end typing or close window
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        bool typing = false;
        for (size_t i = 0; i < widgets.size(); i++) {
            TextField* field = dynamic_cast<TextField*>(widgets[i]);
            if (field && field->focused) typing = true;
        }
        if (!typing) {
            glfwSetWindowShouldClose(window, true);
            return;
        }
    }

    bool typing = false;
    for (size_t i = 0; i < widgets.size(); i++) {
        TextField* field = dynamic_cast<TextField*>(widgets[i]);
        if (field && field->focused) typing = true;
    }

    if (typing) {
        if ((key == GLFW_KEY_ENTER || key == GLFW_KEY_ESCAPE) && action == GLFW_PRESS) {
            for (size_t i = 0; i < widgets.size(); i++) {
                TextField* field = dynamic_cast<TextField*>(widgets[i]);
                if (field) field->focused = false;
            }
            return;
        }
        for (size_t i = 0; i < widgets.size(); i++) widgets[i]->on_key(key, action);
        return;
    }
    
    // A/D Adjust azimuth
    if (key == GLFW_KEY_A) {
        azimuth += daz;
        if (azimuth > 360.0) {
            azimuth -= 360.0;
        }
    } else if (key == GLFW_KEY_D) {
        azimuth -= daz;
        if (azimuth < 0.0)
        {
            azimuth += 360.0;
        }
    }

    // W/S Adjust elevation angle
    if (key == GLFW_KEY_W)
    {
        elevation += del;
        if (elevation > 180.0)
        {
            elevation = 179.0;
        }
    }
    else if (key == GLFW_KEY_S)
    {
        elevation -= del;
        if (elevation < 0.0)
        {
            elevation = 1.0;
        }
    }

    // X/Z Adjust radius (zoom)
    if (key == GLFW_KEY_X)
    {
        radius += dr;
    }
    else if (key == GLFW_KEY_Z)
    {
        radius -= dr;
        if (radius < min_radius)
        {
            radius = min_radius;
        }
    }

    // Compute updated camera position
    GLfloat x, y, z;
    x = (GLfloat)(radius*sin(deg2rad(azimuth))*sin(deg2rad(elevation)));
    y = (GLfloat)(radius*cos(deg2rad(elevation)));
    z = (GLfloat)(radius*cos(deg2rad(azimuth))*sin(deg2rad(elevation)));
    eye = vec3(x,y,z);
}

void mouse_callback(GLFWwindow *window, int button, int action, int mods) {
    if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_PRESS) return;

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    float mx = (float)xpos;
    float my = (float)hh - (float)ypos;

    int hitIndex = -1;
    for (int i = (int)widgets.size() - 1; i >= 0; i--) {
        if (widgets[i]->hit(mx, my)) { hitIndex = i; break; }
    }
    for (size_t i = 0; i < widgets.size(); i++) {
        TextField* field = dynamic_cast<TextField*>(widgets[i]);
        if (field) field->focused = ((int)i == hitIndex);
    }
    if (hitIndex >= 0) widgets[hitIndex]->on_click();
}


void char_callback(GLFWwindow*, unsigned int codepoint) {
    for (size_t i = 0; i < widgets.size(); i++) widgets[i]->on_char(codepoint);
}

void cursor_callback(GLFWwindow *window, double xpos, double ypos) {
    float mx = (float)xpos;
    float my = (float)hh - (float)ypos;
    for (size_t i = 0; i < widgets.size(); i++) {
        widgets[i]->hovered = widgets[i]->hit(mx, my);

    }
}


void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);

    ww = width;
    hh = height;
}
