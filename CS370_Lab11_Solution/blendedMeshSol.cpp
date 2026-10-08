#include <stdio.h>
#include <vector>
#include "../common/GLFWutils.h"
#include "../common/objloader.h"
#include "../common/vmath.h"
#include "../common/lighting.h"
#include "Globals.h"
#include "geometry.h"
#include "lights.h"
#include "materials.h"
#include "drawObjects.h"
#include "shaders.h"
#include "callbacks.h"

void display( );
void render_scene( );

int main(int argc, char**argv)
{
	// Create OpenGL window
	GLFWwindow* window = CreateWindow("Blended Cone", ww, hh);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    } else {
        printf("OpenGL window successfully created\n");
    }

    // Store initial window size
    glfwGetFramebufferSize(window, &ww, &hh);

    // Register callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window,key_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);

	// Create geometry buffers
    build_geometry();
    // Create material buffers
    build_materials();
    // Create light buffers
    build_lights();
	// Create shaders
	build_shaders();

    // Enable depth test
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    // TODO: Enable alpha blending and set blend factors
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Start loop
    while ( !glfwWindowShouldClose( window ) ) {
    	// Draw graphics
        display();
        // Update other events like input handling
        glfwPollEvents();
        GLdouble curTime = glfwGetTime();
        GLdouble dT = curTime - elTime;
        // Bounce sphere
        if (bounce_sphere)
        {
            sphere_pos[1] += sphere_dir*SPHERE_STEP*dT;
            if (sphere_pos[1] > SPHERE_MAX) {
                sphere_pos[1] = SPHERE_MAX - 0.05f;
                sphere_dir *= -1;
            } else if (sphere_pos[1] < SPHERE_MIN) {
                sphere_pos[1] = SPHERE_MIN + 0.05f;
                sphere_dir *= -1;
            }

        }

        // TODO: Roll torus
        if (roll_torus)
        {
            torus_theta += TORUS_STEP*dT;
            if (torus_theta > 360.0f)
            {
                torus_theta -= 360.0f;
            }

        }
        elTime = curTime;
        // Swap buffer onto screen
        glfwSwapBuffers( window );
    }

    // Close window
    glfwTerminate();
    return 0;
}

void display( )
{
    proj_matrix = mat4().identity();
    camera_matrix = mat4().identity();

	// Clear window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set projection matrix
    // Set orthographic viewing volume anisotropic
    GLfloat xratio = 1.0f;
    GLfloat yratio = 1.0f;
    // If taller than wide adjust y
    if (ww <= hh)
    {
        yratio = (GLfloat)hh / (GLfloat)ww;
    }
        // If wider than tall adjust x
    else if (hh <= ww)
    {
        xratio = (GLfloat)ww / (GLfloat)hh;
    }
    // Set projection matrix
    proj_matrix = frustum(-4.0f*xratio, 4.0f*xratio, -4.0f*yratio, 4.0f*yratio, 2.0f, 6.0f);

    // Set camera matrix
    camera_matrix = lookat(eye, center, up);

    // Render objects
	render_scene();

	glFlush();
}

void render_scene( ) {
    model_matrix = mat4().identity();
	mat4 scale_matrix = mat4().identity();
	mat4 rot_matrix = mat4().identity();
	mat4 trans_matrix = mat4().identity();
	mat4 tor_rot_matrix = mat4().identity();

    // Set cube transformation matrix
    trans_matrix = translate(0.0f, -2.1f, 0.0f);
    scale_matrix = scale(6.0f, 0.2f, 6.0f);
    model_matrix = trans_matrix*scale_matrix;
    // Compute normal matrix from model matrix
    normal_matrix = model_matrix.inverse().transpose();
    // Draw object
    draw_mat_object(Cube, WhitePlastic);

    // Set sphere transformation matrix
    trans_matrix = translate(sphere_pos);
    scale_matrix = scale(1.0f, 1.0f, 1.0f);
    model_matrix = trans_matrix*scale_matrix;
    // Compute normal matrix from model matrix
    normal_matrix = model_matrix.inverse().transpose();
    // Draw object
    draw_mat_object(Sphere, RedPlastic);

    // Set torus transformation matrix
    trans_matrix = translate(torus_pos);
    scale_matrix = scale(0.5f, 0.5f, 0.5f);
    rot_matrix = rotate(torus_theta, vec3(0.0f, 1.0f, 0.0f));
    tor_rot_matrix = rotate(90.0f, vec3(1.0f, 0.0f, 0.0f));
    model_matrix = scale_matrix*rot_matrix*trans_matrix*tor_rot_matrix;
    // Compute normal matrix from model matrix
    normal_matrix = model_matrix.inverse().transpose();
    // Draw object
    draw_mat_object(Torus, Brass);

    // Set (translucent) cylinder transformation matrix
    trans_matrix = translate(vec3(0.0f, -2.0f, 0.0f));
    scale_matrix = scale(1.0f, 1.0f, 1.0f);
    model_matrix = trans_matrix*scale_matrix;
    // Compute normal matrix from model matrix
    normal_matrix = model_matrix.inverse().transpose();
    // TODO: Draw translucent object (disable depth mask)
    glDepthMask(GL_FALSE);
    draw_mat_object(Cone, RedAcrylic);
    glDepthMask(GL_TRUE);
}
