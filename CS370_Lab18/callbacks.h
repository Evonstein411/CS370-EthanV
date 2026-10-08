void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // Escape exits program
    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, true);
    }

    // Space toggles animation
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        animate = !animate;
    }

    // Enter toggles multitexture
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
        dirty = !dirty;
    }

    // Up/down adjusts texture mix factor
    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        mix += 0.1f;
        if (mix > 1.0f) {
            mix = 1.0f;
        }
    } else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        mix -= 0.1f;
        if (mix < 0.0f) {
            mix = 0.0f;
        }
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

    // B toggles bump mapping
    if (key == GLFW_KEY_B && action == GLFW_PRESS) {
        bump = !bump;
    }

    // G cycles between normal maps
    if (key == GLFW_KEY_G && action == GLFW_PRESS) {
        golf = (golf+1)%NumGolfNorms;
    }

    // Compute updated camera position
    GLfloat x, y, z;
    x = (GLfloat)(radius*sin(deg2rad(azimuth))*sin(deg2rad(elevation)));
    y = (GLfloat)(radius*cos(deg2rad(elevation)));
    z = (GLfloat)(radius*cos(deg2rad(azimuth))*sin(deg2rad(elevation)));
    eye = vec3(x,y,z);
}

void mouse_callback(GLFWwindow *window, int button, int action, int mods){

}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);

    ww = width;
    hh = height;
}
