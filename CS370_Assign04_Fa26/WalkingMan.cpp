// CS370 Assignment 4 - Walking Man
// Fall 2026


// Your name
//Ethan VonStein
// -------------------------------------------------
// DOCUMENT YOUR UI CONTROLS AND ANY CREATIVITY HERE
//
//
// -------------------------------------------------


#include <stdio.h>
#include <vector>
#include "../common/GLFWutils.h"
#include "../common/textureutils.h"
#include "../common/objloader.h"
#include "../common/vmath.h"
#include "../common/lighting.h"
#include "Globals.h"
#include "geometry.h"
#include "lights.h"
#include "materials.h"
#include "textures.h"
#include "drawObjects.h"
#include "shaders.h"
#include "scenegraph.h"
#include "callbacks.h"

void display( );
void render_scene( );
void draw_background(GLuint b_obj, GLuint b_tex);

int main(int argc, char**argv)
{
	// Create OpenGL window
	GLFWwindow* window = CreateWindow("Go Spartans! 2026", ww, hh);
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
    // Create textures
    build_textures();
    // Create shaders
    build_shaders();
    // Create scene graph (after building shaders)
    build_scene_graph();

    // Enable depth test
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    // Set background color
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    // Enable blending and set blend factors
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set initial camera position
	GLfloat x, y, z;
	x = (GLfloat)(radius*sin(deg2rad(azimuth))*sin(deg2rad(elevation)));
	y = (GLfloat)(radius*cos(deg2rad(elevation)));
	z = (GLfloat)(radius*cos(deg2rad(azimuth))*sin(deg2rad(elevation)));
	eye = vec3(x,y,z);

    // Start loop
    while ( !glfwWindowShouldClose( window ) ) {
    	// Draw graphics
        display();
        // Update other events like input handling
        glfwPollEvents();
        GLdouble curTime = glfwGetTime();
        GLdouble dt = curTime - elTime;
        if (animate) {
        	// TODO: Update scene graph nodes for animation
            head_theta += (GLfloat)(dt * 45.0);
            if (head_theta > 360.0f) head_theta -= 360.0f;
            head.set_update_transform(
               translate(0.0f, TORSO_HEIGHT * 0.5f + HEAD_HEIGHT, 0.0f)*
               rotate(180.0f, vec3(1.0f, 0.0f, 0.0f)) *
               rotate(head_theta, vec3(0.0f, 1.0f, 0.0f))
            );

            upper_left_arm_phi  += (GLfloat)(dt * 60.0);
            upper_right_arm_phi -= (GLfloat)(dt * 60.0);
            if (upper_left_arm_phi  > 360.0f) upper_left_arm_phi  -= 360.0f;
            if (upper_right_arm_phi < 0.0f)   upper_right_arm_phi += 360.0f;

            left_upper_arm.set_update_transform(
                translate(TORSO_WIDTH * 0.5f + UPPER_ARM_WIDTH * 0.5f, TORSO_HEIGHT * 0.4f, 0.0f)
                * rotate(upper_left_arm_phi, vec3(1.0f, 0.0f, 0.0f))
            );
            right_upper_arm.set_update_transform(
                translate(-(TORSO_WIDTH * 0.5f + UPPER_ARM_WIDTH * 0.5f), TORSO_HEIGHT * 0.4f, 0.0f)
                * rotate(upper_right_arm_phi, vec3(1.0f, 0.0f, 0.0f))
            );


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

	draw_background(Background, YCP);

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
    proj_matrix = ortho(-15.0f*xratio, 15.0f*xratio, -10.0f*yratio, 20.0f*yratio, -50.0f, 50.0f);

    // Set camera matrix
    camera_matrix = lookat(eye, center, up);

    // Render objects
	render_scene();

	glFlush();
}

void render_scene( ) {
    // TODO: Render scene graph
    traverse_scene_graph(&torso, mat4().identity());
}

void draw_background(GLuint b_obj, GLuint b_texid){
    // TODO: Draw background image
    mat4 scale_m = scale(1.0f, 1.0f, 1.0f);
    mat4 trans_m = translate(0.0f, 0.0f, 1.0f);
    model_matrix = trans_m * scale_m;

    glDisable(GL_DEPTH_TEST);
    draw_tex_object(b_obj, b_texid);
    glEnable(GL_DEPTH_TEST);
}
