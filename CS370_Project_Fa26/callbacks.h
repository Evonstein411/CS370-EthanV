extern SceneObject* selectedObject;

//exists in houseproject forward dec to avoid error
void update_fields();

//functions
void cursor_to_framebuffer(GLFWwindow* window, double xpos, double ypos, float& mx, float& my);


//keyboard callback
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    //press escape either end typing or close window
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        bool typing = false;
        for (size_t i = 0; i < widgets.size(); i++) {
            TextFieldBase* field = dynamic_cast<TextFieldBase*>(widgets[i]);
            if (field && field->focused) typing = true;
        }
        if (!typing) {
            glfwSetWindowShouldClose(window, true);
            return;
        }
    }

    bool typing = false;
    for (size_t i = 0; i < widgets.size(); i++) {
        TextFieldBase* field = dynamic_cast<TextFieldBase*>(widgets[i]);
        if (field && field->focused) typing = true;
    }

    if (typing) {
        if ((key == GLFW_KEY_ENTER || key == GLFW_KEY_ESCAPE) && action == GLFW_PRESS) {
            for (size_t i = 0; i < widgets.size(); i++) {
                TextFieldBase* field = dynamic_cast<TextFieldBase*>(widgets[i]);
                if (field && field->focused) field->commit();
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

//mouse callback
void mouse_callback(GLFWwindow *window, int button, int action, int mods) {
    if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_PRESS) return;

    //update mouse position
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    float mx, my;
    cursor_to_framebuffer(window, xpos, ypos, mx, my);

    //check if clicked on widget
    int hitIndex = -1;
    for (int i = (int)widgets.size() - 1; i >= 0; i--) {
        if (widgets[i]->hit(mx, my)) {
            hitIndex = i;
            break;
        }
    }



    //check text fields
    for (size_t i = 0; i < widgets.size(); i++) {
        TextFieldBase* field = dynamic_cast<TextFieldBase*>(widgets[i]);
        if(field) {
            //if focused and not clicked commit
            if (field->focused && (int)i != hitIndex)
                field->commit();
            field->focused = (field->editable && (int)i == hitIndex);
        }
    }

    //call on click for widget clicked on
    if (hitIndex >= 0) {
        widgets[hitIndex]->on_click();
        return;
    }

    //check if clicked on object
    SceneObject* clicked = pick_scene_object(mx, my);
    selectedObject = clicked;
    update_fields();


}


void char_callback(GLFWwindow*, unsigned int codepoint) {
    for (size_t i = 0; i < widgets.size(); i++) widgets[i]->on_char(codepoint);
}

//mouse move callback
void cursor_callback(GLFWwindow *window, double xpos, double ypos) {
    float mx, my;
    cursor_to_framebuffer(window, xpos, ypos, mx, my);
    for (size_t i = 0; i < widgets.size(); i++) {
        widgets[i]->hovered = widgets[i]->hit(mx, my);
        widgets[i]->on_hover(mx, my);
    }
}


//update on window resize
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    ww = width;
    hh = height;
}

//ensure cursor matching size of window and proper scaling
void cursor_to_framebuffer(GLFWwindow* window, double xpos, double ypos, float& mx, float& my) {
    int win_w, win_h;
    glfwGetWindowSize(window, &win_w, &win_h);
    if (win_w <= 0 || win_h <= 0) {
        mx = (float)xpos;
        my = (float)hh - (float)ypos;
        return;
    }
    float xscale = (float)ww / (float)win_w;
    float yscale = (float)hh / (float)win_h;
    mx = (float)xpos * xscale;
    my = (float)hh - (float)ypos * yscale;
}
