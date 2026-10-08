void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // Esc closes window
    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, true);
    }

    // Space toggles animation
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        animate = !animate;
    }

    // TODO: Move hexagon with arrow keys
    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        hex_y += delta;
    } else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        hex_y -= delta;
    } else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        hex_x -= delta;
    } else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        hex_x += delta;
    }

}

void mouse_callback(GLFWwindow *window, int button, int action, int mods){
    // TODO: Flip spin direction with mouse click
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        dir *= -1;
    }
}
