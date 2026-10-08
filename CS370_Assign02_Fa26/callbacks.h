void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // Escape exits program
    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, true);
    }

    // Space toggles animation
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        animate = !animate;
    }
    
    // Toggle projection mode
    if (key == GLFW_KEY_O)
    {
        proj = ORTHOGRAPHIC;
    }
    else if (key == GLFW_KEY_P)
    {
        proj = PERSPECTIVE;
    }

    if (proj == ORTHOGRAPHIC) {
        if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT) {
            azimuth += daz;
            if (azimuth > 360.0) {
                azimuth -= 360.0;
            }
        } else if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) {
            azimuth -= daz;
            if (azimuth < 0.0)
            {
                azimuth += 360.0;
            }
        }

        // Adjust elevation angle
        if (key == GLFW_KEY_W || key == GLFW_KEY_UP)
        {
            elevation += del;
            if (elevation > 180.0)
            {
                elevation = 179.0;
            }
        }
        else if (key == GLFW_KEY_S || key == GLFW_KEY_DOWN)
        {
            elevation -= del;
            if (elevation < 0.0)
            {
                elevation = 1.0;
            }
        }


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




    }//end ortho



}//end key callback

void mouse_callback(GLFWwindow *window, int button, int action, int mods){

}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);

    ww = width;
    hh = height;
}
