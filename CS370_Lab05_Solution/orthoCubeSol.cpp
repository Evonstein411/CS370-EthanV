#include <stdio.h>
#include <vector>
#include "../common/GLFWutils.h"
#include "../common/vmath.h"
#include "Globals.h"
#include "geometry.h"
#include "drawObjects.h"
#include "shaders.h"
#include "callbacks.h"

void display( );
void render_scene( );

int main(int argc, char**argv)
{
	// Create OpenGL window
	GLFWwindow* window = CreateWindow("Ortho Cube", ww, hh);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    } else {
        printf("OpenGL window successfully created\n");
    }

    // TODO: Store initial window size in global variables
    glfwGetFramebufferSize(window, &ww, &hh);

    // Register callbacks
    glfwSetKeyCallback(window,key_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);
    // TODO: Register resize callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Get initial time
    elTime = glfwGetTime();

    // Create geometry buffers
    build_geometry();
	// Create shaders
	build_shaders();

    // TODO: Enable depth test
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

	// Start loop
    while ( !glfwWindowShouldClose( window ) ) {
    	// Draw graphics
        display();
        // Update other events like input handling
        glfwPollEvents();
        // Update angle based on time for fixed rpm
        GLdouble curTime = glfwGetTime();
        if (animate) {
        	cube_angle += (curTime-elTime)*(rpm/60.0)*360.0;
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
    // Declare projection matrix
    proj_matrix = mat4().identity();
    camera_matrix = mat4().identity();  // Default camera matrix for now

	// TODO: Clear window and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glClear(GL_COLOR_BUFFER_BIT);

    // TODO: Compute anisotropic scaling
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

    // TODO: Set projection matrix
    proj_matrix = ortho(-2.0f*xratio, 2.0f*xratio, -2.0f*yratio, 2.0f*yratio, -2.0f, 2.0f);
    //proj_matrix = ortho(-2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f);

    // Render objects
	render_scene();

	// Flush pipeline
	glFlush();
}

void render_scene( ) {
    // Declare transformation matrices
    model_matrix = mat4().identity();
    mat4 scale_matrix = mat4().identity();
    mat4 rot_matrix = mat4().identity();
    mat4 trans_matrix = mat4().identity();

	// Draw cube
    trans_matrix = translate(vec3(0.0f, 0.0f, 0.0f));
    rot_matrix = rotate(cube_angle, normalize(axis));
    scale_matrix = scale(vec3(1.0f, 1.0f, 1.0f));
	model_matrix = trans_matrix*rot_matrix*scale_matrix;
    draw_color_object(Cube, CubeGradient);
}

