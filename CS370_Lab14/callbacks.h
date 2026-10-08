void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // Escape exits program
    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, true);
    }

    // Space toggles animation
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        animate = !animate;
    }

    // Enter toggles spotlight
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
        lightOn[RedSpotLight] = (lightOn[RedSpotLight]+1)%2;
    }

    // a d rotate base
    if (key == GLFW_KEY_D)
    {
        theta += dtheta;
        if (theta > 360.0f)
        {
            theta -= 360.0f;
        }
        // TODO: Update base


    }
    else if (key == GLFW_KEY_A)
    {
        theta -= dtheta;
        if (theta < 0.0f)
        {
            theta += 360.0f;
        }
        // TODO: Update base


    }

    // w s rotates lower arm
    if (key == GLFW_KEY_S)
    {
        phi += dphi;
        if (phi > 90.0f)
        {
            phi = 90.0f;
        }
        // TODO: Update lower arm


    }
    else if (key == GLFW_KEY_W)
    {
        phi -= dphi;
        if (phi < -90.0f)
        {
            phi = -90.0f;
        }
        // TODO: Update lower arm


    }

    // m n rotates left upper arm
    if (key == GLFW_KEY_N)
    {
        left_psi += dpsi;
        if (left_psi > 180.0f)
        {
            left_psi = 180.0f;
        }
        // TODO: Update left upper arm


    }
    else if (key == GLFW_KEY_M)
    {
        left_psi -= dpsi;
        if (left_psi < -180.0f)
        {
            left_psi = -180.0f;
        }
        // TODO: Update left upper arm


    }

    // . , rotates right upper arm
    if (key == GLFW_KEY_COMMA)
    {
        right_psi += dpsi;
        if (right_psi > 180.0f)
        {
            right_psi = 180.0f;
        }
        // TODO: Update right upper arm


    }
    else if (key == GLFW_KEY_PERIOD)
    {
        right_psi -= dpsi;
        if (right_psi < -180.0f)
        {
            right_psi = -180.0f;
        }
        // TODO: Update right upper arm


    }
}

void mouse_callback(GLFWwindow *window, int button, int action, int mods){

}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);

    ww = width;
    hh = height;
}
