void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // Escape exits program
    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, true);
    }

	// TODO: Add object key bindings
    //cube
    if (key == GLFW_KEY_V && action == GLFW_PRESS) {
        cube_slide = !cube_slide;
    }
    //sphere
    if (key == GLFW_KEY_B && action == GLFW_PRESS) {
        sphere_bounce = !sphere_bounce;
    }
    //octahedron
    if (key == GLFW_KEY_N && action == GLFW_PRESS) {
        pyr_spin = !pyr_spin;
    }


    if (key == GLFW_KEY_M && action == GLFW_PRESS) {
        torus_animate = !torus_animate;
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        cube_slide = !cube_slide;
        sphere_bounce = !sphere_bounce;
        pyr_spin = !pyr_spin;
        torus_animate = !torus_animate;
    }


    //point light on off
    if (key == GLFW_KEY_0 && action == GLFW_PRESS) {
        lightOn[WhitePointLight] = !lightOn[WhitePointLight];
    }

    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        lightOn[CubeLight] = !lightOn[CubeLight];
    }

    if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        lightOn[SphereLight] = !lightOn[SphereLight];
    }

    if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
        lightOn[OctLight] = !lightOn[OctLight];
    }

    if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
        lightOn[TorusLight] = !lightOn[TorusLight];
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
