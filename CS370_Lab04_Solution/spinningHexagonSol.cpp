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
	GLFWwindow* window = CreateWindow("Transform Hexagon", ww, hh);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    } else {
        printf("OpenGL window successfully created\n");
    }

    // TODO: Register callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);

    // TODO: Get initial time
    elTime = glfwGetTime();

	// Create geometry buffers
    build_geometry();
	// Create shaders
	build_shaders();    

	// Start loop
    while ( !glfwWindowShouldClose( window ) ) {
    	// Draw graphics
        display();
        // Update other events like input handling
        glfwPollEvents();
       // TODO: Get current time
        GLdouble curTime = glfwGetTime();
        if (animate) {
        	//  TODO: Update angle based on time for fixed rpm
        	hex_angle += dir*(curTime-elTime)*(rpm/60.0)*360.0;
        }
        // TODO: Update elTime
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
    proj_matrix = mat4().identity();    // Default projection matrix for now
    camera_matrix = mat4().identity();  // Default camera matrix for now

	// Clear window
	glClear(GL_COLOR_BUFFER_BIT);

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

    // Draw hexagon
    // TODO: Set translation matrix
    trans_matrix = translate(hex_x, hex_y, 0.0f);

    // TODO: Set rotation matrix
    rot_matrix = rotate(hex_angle, vec3(0.0f, 0.0f, 1.0f));

    scale_matrix = scale(0.5f, 0.5f, 1.0f);
    model_matrix = trans_matrix*rot_matrix*scale_matrix;
    draw_color_object(Hexagon, HexRed);
}
