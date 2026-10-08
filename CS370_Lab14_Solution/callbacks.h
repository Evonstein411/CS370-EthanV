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
        base.set_update_transform(rotate(theta, vec3(0.0f, 1.0f, 0.0f)));
    }
    else if (key == GLFW_KEY_A)
    {
        theta -= dtheta;
        if (theta < 0.0f)
        {
            theta += 360.0f;
        }
        // TODO: Update base
        base.set_update_transform(rotate(theta, vec3(0.0f, 1.0f, 0.0f)));
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
        lower_arm.set_update_transform(translate(vec3(0.0f, BASE_HEIGHT, 0.0f))*rotate(phi, vec3(1.0f, 0.0f, 0.0f)));
    }
    else if (key == GLFW_KEY_W)
    {
        phi -= dphi;
        if (phi < -90.0f)
        {
            phi = -90.0f;
        }
        // TODO: Update lower arm
        lower_arm.set_update_transform(translate(vec3(0.0f, BASE_HEIGHT, 0.0f))*rotate(phi, vec3(1.0f, 0.0f, 0.0f)));
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
        left_upper_arm.set_update_transform(translate(vec3((LOWER_WIDTH+UPPER_WIDTH)/2, LOWER_HEIGHT, 0.0f))*rotate(left_psi, vec3(1.0f, 0.0f, 0.0f)));
    }
    else if (key == GLFW_KEY_M)
    {
        left_psi -= dpsi;
        if (left_psi < -180.0f)
        {
            left_psi = -180.0f;
        }
        // TODO: Update left upper arm
        left_upper_arm.set_update_transform(translate(vec3((LOWER_WIDTH+UPPER_WIDTH)/2, LOWER_HEIGHT, 0.0f))*rotate(left_psi, vec3(1.0f, 0.0f, 0.0f)));
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
        right_upper_arm.set_update_transform(translate(vec3(-(LOWER_WIDTH+UPPER_WIDTH)/2, LOWER_HEIGHT, 0.0f))*rotate(right_psi, vec3(1.0f, 0.0f, 0.0f)));
    }
    else if (key == GLFW_KEY_PERIOD)
    {
        right_psi -= dpsi;
        if (right_psi < -180.0f)
        {
            right_psi = -180.0f;
        }
        // TODO: Update right upper arm
        right_upper_arm.set_update_transform(translate(vec3(-(LOWER_WIDTH+UPPER_WIDTH)/2, LOWER_HEIGHT, 0.0f))*rotate(right_psi, vec3(1.0f, 0.0f, 0.0f)));
    }
}

void mouse_callback(GLFWwindow *window, int button, int action, int mods){

}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);

    ww = width;
    hh = height;
}
