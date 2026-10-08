void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // Esc closes window
    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, true);
    }

    //scaling
    if (key == GLFW_KEY_MINUS && action == GLFW_PRESS) {
        hex_scale -= 0.1;
    }else if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS) {
        hex_scale += 0.1;
    }

    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        if(r > 0.0f){
           r -= 0.1f;
        }else {
            r = 1.0f;
        }
        build_geometry();
    }

    if (key == GLFW_KEY_G && action == GLFW_PRESS) {
        if(g > 0.0f){
            g -= 0.1f;
        }else {
            g = 1.0f;
        }
        build_geometry();

    }

    if (key == GLFW_KEY_B && action == GLFW_PRESS) {
        if(b > 0.0f){
            b -= 0.1f;
        }else {
            b = 1.0f;
        }
        build_geometry();

    }


    // Space toggles animation
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        animate = !animate;
    }

    // TODO: Move hexagon with arrow keys + wasd
    if ((key == GLFW_KEY_UP || key == GLFW_KEY_W) && action == GLFW_PRESS) {
        hex_y += delta;
    } else if ((key == GLFW_KEY_DOWN || key == GLFW_KEY_S) && action == GLFW_PRESS) {
        hex_y -= delta;
    } else if ((key == GLFW_KEY_LEFT || key == GLFW_KEY_A) && action == GLFW_PRESS) {
        hex_x -= delta;
    } else if ((key == GLFW_KEY_RIGHT || key == GLFW_KEY_D) && action == GLFW_PRESS) {
        hex_x += delta;
    }

}

void mouse_callback(GLFWwindow *window, int button, int action, int mods){
    // TODO: Flip spin direction with mouse click
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        dir *= -1;
    }

}
