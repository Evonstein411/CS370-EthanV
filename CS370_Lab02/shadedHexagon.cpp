#include <stdio.h>
#include <vector>
#include "../common/GLFWutils.h"
#include "../common/vmath.h"
#include "Globals.h"
#include "geometry.h"
#include "drawObjects.h"
#include "shaders.h"

void display( );
void render_scene( );

int main(int argc, char**argv)
{
	// Create OpenGL window
	GLFWwindow* window = CreateWindow("Shaded Hexagon", ww, hh);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    } else {
        printf("OpenGL window successfully created\n");
    }

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
    model_matrix = mat4().identity();  // Default model matrix for now

	// Clear window
	glClear(GL_COLOR_BUFFER_BIT);

	// Render objects
	render_scene();

	// Flush pipeline
	glFlush();
}

void render_scene() {
    draw_color_object(Hexagon, HexGradient);
}