void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // Esc closes window
    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, true);
    }

    // TODO: Space toggles animation
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        animate = !animate;
    }
    
}

void mouse_callback(GLFWwindow *window, int button, int action, int mods){
    // TODO: Flip spin direction with mouse click
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        dir *= -1;
    }

}
