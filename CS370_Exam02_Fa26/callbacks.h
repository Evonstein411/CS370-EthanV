void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // Escape exits program
    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, true);
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        mode++;
        if (mode > NUM_MODES)
        {
            mode = 0;
            spinning = false;
            tipping = false;
            pouring = false;
        }
        if (mode == 0 || mode == 1 || mode == 5 || mode == 7) {
            Lights.clear();
            build_lights();
        }
        if (mode == 2 || mode == 3 || mode == 7 || mode == 9) {
            Materials.clear();
            build_materials();
        }
        if (mode == 6) {
            spinning = true;
        }
        if (mode == 8) {
            tipping = true;
        }

    }

    // Animation keypresses
    // Spinning plate
    if (key == GLFW_KEY_P && action == GLFW_PRESS && mode > 5) {
        spinning = !spinning;
    }
    // Tipping teapot
    if (key == GLFW_KEY_T && action == GLFW_PRESS && mode > 7) {
        tipping = !tipping;
    }


    // Adjust azimuth
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

    // Adjust elevation angle
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
