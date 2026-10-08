void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // Escape exits program
    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, true);
    }

    // Space toggles animation
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        animate = !animate;
    }
}

void mouse_callback(GLFWwindow *window, int button, int action, int mods){

}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);

	// TODO: Store new window size
    ww = width;
    hh = height;
}
