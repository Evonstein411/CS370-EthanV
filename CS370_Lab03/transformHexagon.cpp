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
	GLFWwindow* window = CreateWindow("Transform Hexagon", ww, hh);
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

	// Draw base hexagon (red)
	model_matrix = mat4().identity();
    draw_color_object(Hexagon, HexRed);

    // TODO: Draw scaled hexagon
	scale_matrix = scale(0.5f, 0.5f, 0.5f);
	model_matrix = scale_matrix;
	draw_color_object(Hexagon, HexGreen);


    // TODO: Draw scaled/rotated hexagon
	rot_matrix = rotate(90.0f, 0.0f, 0.0f, 1.0f);
	scale_matrix = scale(0.25f, 0.25f, 0.25f);
	model_matrix = rot_matrix*scale_matrix;
	draw_color_object(Hexagon, HexBlue);

    // TODO: Draw scaled/rotated/translated hexagon
	trans_matrix = translate(0.5f, 0.5f, 0.0f);
	rot_matrix = rotate(90.0f, 0.0f, 0.0f, 1.0f);
	scale_matrix = scale(0.25f, 0.25f, 0.25f);
	model_matrix = trans_matrix*rot_matrix*scale_matrix;
	draw_color_object(Hexagon, HexYellow);

	trans_matrix = translate(-0.5f, -0.5f, 0.0f);
	rot_matrix = rotate(90.0f, 0.0f, 0.0f, 1.0f);
	scale_matrix = scale(0.25f, 0.25f, 0.25f);
	model_matrix = trans_matrix*rot_matrix*scale_matrix;
	draw_color_object(Hexagon, HexPurple);




/*
    mat4 shear = mat4().identity();
    shear[0][1] = 0.5f;
    scale_matrix = scale(0.5f, 0.5f, 1.0f);
    model_matrix = shear*scale_matrix;
    draw_color_object(Hexagon, HexYellow);
*/

}
