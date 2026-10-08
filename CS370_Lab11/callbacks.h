void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // Escape exits program
    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, true);
    }

	// Toggle animations
    if (key == GLFW_KEY_B && action == GLFW_PRESS) {
        bounce_sphere = !bounce_sphere;
    } else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        roll_torus = !roll_torus;
    }
}

void mouse_callback(GLFWwindow *window, int button, int action, int mods){

}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);

    ww = width;
    hh = height;
}
