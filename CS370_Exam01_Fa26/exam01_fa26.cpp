// CS370 - Exam 1
// Fall 2026

/******************************************/
/*       INSERT (a) CODE HERE             */
/******************************************/
// Ethan VonStein

#include <stdio.h>
#include <vector>
#include "../common/GLFWutils.h"
#include "../common/objloader.h"
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
	GLFWwindow* window = CreateWindow("Exam 1 Fall 2026", ww, hh);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    } else {
        printf("OpenGL window successfully created\n");
    }

    // Store initial window size in global variables
    glfwGetFramebufferSize(window, &ww, &hh);

    // Register callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window,key_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);

    // Create geometry buffers
    build_geometry();
	// Create shaders
	build_shaders();

    // Enable depth test
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    // Set background color
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    // Set Initial camera position
	GLfloat x, y, z;
	x = (GLfloat)(radius*sin(deg2rad(azimuth))*sin(deg2rad(elevation)));
	y = (GLfloat)(radius*cos(deg2rad(elevation)));
	z = (GLfloat)(radius*cos(deg2rad(azimuth))*sin(deg2rad(elevation)));
	eye = vec3(x,y,z);

    // Set initial time
    elTime = glfwGetTime();

    // Start loop
    while ( !glfwWindowShouldClose( window ) ) {
    	// Draw graphics
        display();
        // Update other events like input handling
        glfwPollEvents();
        // Update angle based on time for fixed rpm
        GLdouble curTime = glfwGetTime();
        if (mode > 3)
        {
            if (spin_flag)
            {
                /******************************************/
                /*       INSERT (h) CODE HERE             */
                /******************************************/
                // TODO: Set spin_theta
                spin_theta += (curTime-elTime)*(rpm/60.0)*360.0;
                rev_theta += (curTime-elTime)*(rpm/60.0)*360.0;

                
            }
            if (mode > 4)
            {
                if (rev_flag)
                {
                    /******************************************/
                    /*       INSERT (i) CODE HERE             */
                    /******************************************/
                    // TODO: Set rev_theta
                    
                }
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
    // Declare projection and camera matrices
    proj_matrix = mat4().identity();
    camera_matrix = mat4().identity();

    // Clear window and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Compute anisotropic scaling
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
    /******************************************/
    /*       INSERT (b) CODE HERE             */
    /******************************************/
    // TODO: Set projection matrix
    proj_matrix = ortho(-6.0f*xratio, 6.0f*xratio, -6.0f*yratio, 6.0f*yratio, -10.0, 10.0);
    // TODO: Set camera matrix
    camera_matrix = lookat(eye, center, up);


    // Render objects
	render_scene();

	// Flush pipeline
	glFlush();
}

void render_scene( ) {
    /*********************************************************/
    /* TODO: Declare additional transformation matrices here */
    /*********************************************************/
    model_matrix = mat4().identity();
    mat4 scale_matrix = mat4().identity();
    mat4 rot_matrix = mat4().identity();
    mat4 rot2_matrix = mat4().identity();
    mat4 rot3_matrix = mat4().identity();
    mat4 trans_matrix = mat4().identity();
    mat4 trans2_matrix = mat4().identity();
    mat4 trans3_matrix = mat4().identity();

    draw_table();
    // (c) Pyramid instance model
    if (mode == 1) {
        draw_color_object(Pyramid, PyramidGrad);
    }
    // (d) Basic pyramid centered at origin
    else if (mode==2)
    {
        /******************************************/
        /*       INSERT (d) CODE HERE             */
        /******************************************/
        // TODO: Add pyramid transformations
        trans_matrix = translate(-0.5f, 0.0f, -0.5f);
        model_matrix = trans_matrix;
        draw_color_object(Pyramid, PyramidGrad);
    }
    // (e) Two scaled pyramids and sphere located on circle
    else if (mode==3)
    {
        /******************************************/
        /*       INSERT (f) CODE HERE             */
        /******************************************/
        // TODO: Add pyramid transformations
        trans_matrix = translate(-0.5f, 0.0f, -0.5f);
        scale_matrix = scale(1.0f, 2.0f, 1.0f);
        trans2_matrix = translate(-CIRCLE_RAD, 0.0f, 0.0f);
        model_matrix = trans2_matrix*scale_matrix*trans_matrix;
        draw_color_object(Pyramid, PyramidGrad);

        // TODO: Add second pyramid transformations       
        trans_matrix = translate(-0.5f, 0.0f, -0.5f);
        scale_matrix = scale(1.0f, 2.0f, 1.0f);
        trans2_matrix = translate(CIRCLE_RAD, 0.0f, 0.0f);
        model_matrix = trans2_matrix*scale_matrix*trans_matrix;
        draw_color_object(Pyramid, PyramidGrad);


        // TODO: Add sphere
        radius = sqrt(1/M_PI);
        trans_matrix = translate(0.0f, radius, 0.0f);
        scale_matrix = scale(radius, radius, radius);
        model_matrix = trans_matrix*scale_matrix;
        draw_color_object(Sphere, SphereMagenta);
       
    }
    // (f) Two scaled pyramids located on circle at target location
    else if (mode==4)
    {
        /******************************************/
        /*       INSERT (g) CODE HERE             */
        /******************************************/
        // TODO: Add pyramid transformations
        trans_matrix = translate(-0.5f, 0.0f, -0.5f);
        scale_matrix = scale(1.0f, 2.0f, 1.0f);
        trans2_matrix = translate(-CIRCLE_RAD, 0.0f, 0.0f);
        trans3_matrix = translate(TARGET_X, TARGET_Y, TARGET_Z);
        model_matrix = trans3_matrix*trans2_matrix*scale_matrix*trans_matrix;
        draw_color_object(Pyramid, PyramidGrad);

        // TODO: Add second pyramid transformations
        trans_matrix = translate(-0.5f, 0.0f, -0.5f);
        scale_matrix = scale(1.0f, 2.0f, 1.0f);
        trans2_matrix = translate(CIRCLE_RAD, 0.0f, 0.0f);
        trans3_matrix = translate(TARGET_X, TARGET_Y, TARGET_Z);
        model_matrix = trans3_matrix*trans2_matrix*scale_matrix*trans_matrix;
        draw_color_object(Pyramid, PyramidGrad);


        // TODO: Add sphere
        radius = sqrt(1/M_PI);
        trans_matrix = translate(0.0f, radius, 0.0f);
        scale_matrix = scale(radius, radius, radius);
        trans2_matrix = translate(TARGET_X, TARGET_Y, TARGET_Z);
        model_matrix = trans2_matrix*trans_matrix*scale_matrix;
        draw_color_object(Sphere, SphereMagenta);


    }
    // (h) Spinning pyramids
    else if (mode==5)
    {
        /******************************************/
        /*       INSERT (h) CODE HERE          */
        /******************************************/
        // TODO: Add pyramid transformations
        trans_matrix = translate(-0.5f, 0.0f, -0.5f);
        scale_matrix = scale(1.0f, 2.0f, 1.0f);
        trans2_matrix = translate(-CIRCLE_RAD, 0.0f, 0.0f);
        trans3_matrix = translate(TARGET_X, TARGET_Y, TARGET_Z);
        rot_matrix = rotate(spin_theta, vec3(0.0f, 1.0f, 0.0f));
        model_matrix = trans3_matrix*trans2_matrix*rot_matrix*scale_matrix*trans_matrix;
        draw_color_object(Pyramid, PyramidGrad);

        // TODO: Add second pyramid transformations
        trans_matrix = translate(-0.5f, 0.0f, -0.5f);
        scale_matrix = scale(1.0f, 2.0f, 1.0f);
        trans2_matrix = translate(CIRCLE_RAD, 0.0f, 0.0f);
        trans3_matrix = translate(TARGET_X, TARGET_Y, TARGET_Z);
        rot_matrix = rotate(-spin_theta, vec3(0.0f, 1.0f, 0.0f));
        model_matrix = trans3_matrix*trans2_matrix*rot_matrix*scale_matrix*trans_matrix;
        draw_color_object(Pyramid, PyramidGrad);


        // TODO: Add sphere
        radius = sqrt(1/M_PI);
        trans_matrix = translate(0.0f, radius, 0.0f);
        scale_matrix = scale(radius, radius, radius);
        trans2_matrix = translate(TARGET_X, TARGET_Y, TARGET_Z);
        model_matrix = trans2_matrix*trans_matrix*scale_matrix;
        draw_color_object(Sphere, SphereMagenta);

    }
    // (i) Spinning and revolving pyramids
    else if (mode==6)
    {
        /******************************************/
        /*       INSERT (i) CODE HERE          */
        /******************************************/
        // TODO: Add pyramid transformations
        trans_matrix = translate(-0.5f, 0.0f, -0.5f);
        scale_matrix = scale(1.0f, 2.0f, 1.0f);
        trans2_matrix = translate(-CIRCLE_RAD, 0.0f, 0.0f);
        trans3_matrix = translate(TARGET_X, TARGET_Y, TARGET_Z);
        rot_matrix = rotate(spin_theta, vec3(0.0f, 1.0f, 0.0f));
        rot2_matrix = rotate(rev_theta, vec3(0.0f, 1.0f, 0.0f));
        model_matrix = trans3_matrix*rot2_matrix*trans2_matrix*rot_matrix*scale_matrix*trans_matrix;
        draw_color_object(Pyramid, PyramidGrad);

        // TODO: Add second pyramid transformations
        trans_matrix = translate(-0.5f, 0.0f, -0.5f);
        scale_matrix = scale(1.0f, 2.0f, 1.0f);
        trans2_matrix = translate(CIRCLE_RAD, 0.0f, 0.0f);
        trans3_matrix = translate(TARGET_X, TARGET_Y, TARGET_Z);
        rot_matrix = rotate(-spin_theta, vec3(0.0f, 1.0f, 0.0f));
        rot2_matrix = rotate(-rev_theta, vec3(0.0f, 1.0f, 0.0f));
        model_matrix = trans3_matrix*rot2_matrix*trans2_matrix*rot_matrix*scale_matrix*trans_matrix;
        draw_color_object(Pyramid, PyramidGrad);


        // TODO: Add sphere
        radius = sqrt(1/M_PI);
        trans_matrix = translate(0.0f, radius, 0.0f);
        scale_matrix = scale(radius, radius, radius);
        trans2_matrix = translate(TARGET_X, TARGET_Y, TARGET_Z);
        model_matrix = trans2_matrix*trans_matrix*scale_matrix;
        draw_color_object(Sphere, SphereMagenta);

    }
    // (j) EXTRA CREDIT: Add sphere on top of pyramids
    else if (mode==7)
    {
        /******************************************/
        /*       INSERT (i) CODE HERE          */
        /******************************************/
        // TODO: Add pyramid transformations
        trans_matrix = translate(-0.5f, 0.0f, -0.5f);
        scale_matrix = scale(1.0f, 2.0f, 1.0f);
        trans2_matrix = translate(-CIRCLE_RAD, 0.0f, 0.0f);
        trans3_matrix = translate(TARGET_X, TARGET_Y, TARGET_Z);
        rot_matrix = rotate(spin_theta, vec3(0.0f, 1.0f, 0.0f));
        rot2_matrix = rotate(rev_theta, vec3(0.0f, 1.0f, 0.0f));
        model_matrix = trans3_matrix*rot2_matrix*trans2_matrix*rot_matrix*scale_matrix*trans_matrix;
        draw_color_object(Pyramid, PyramidGrad);

        // TODO: Add second pyramid transformations
        trans_matrix = translate(-0.5f, 0.0f, -0.5f);
        scale_matrix = scale(1.0f, 2.0f, 1.0f);
        trans2_matrix = translate(CIRCLE_RAD, 0.0f, 0.0f);
        trans3_matrix = translate(TARGET_X, TARGET_Y, TARGET_Z);
        rot_matrix = rotate(-spin_theta, vec3(0.0f, 1.0f, 0.0f));
        rot2_matrix = rotate(-rev_theta, vec3(0.0f, 1.0f, 0.0f));
        model_matrix = trans3_matrix*rot2_matrix*trans2_matrix*rot_matrix*scale_matrix*trans_matrix;
        draw_color_object(Pyramid, PyramidGrad);


        // TODO: Add sphere
        radius = sqrt(1/M_PI);
        trans_matrix = translate(0.0f, radius, 0.0f);
        scale_matrix = scale(radius, radius, radius);
        trans2_matrix = translate(-CIRCLE_RAD, 2.0f, 0.0f);
        trans3_matrix = translate(TARGET_X, TARGET_Y, TARGET_Z);
        rot2_matrix = rotate(rev_theta, vec3(0.0f, 1.0f, 0.0f));
        model_matrix = trans3_matrix*rot2_matrix*trans2_matrix*scale_matrix*trans_matrix;
        draw_color_object(Sphere, SphereMagenta);


        radius = sqrt(1/M_PI);
        trans_matrix = translate(0.0f, radius, 0.0f);
        scale_matrix = scale(radius, radius, radius);
        trans2_matrix = translate(CIRCLE_RAD, 2.0f, 0.0f);
        trans3_matrix = translate(TARGET_X, TARGET_Y, TARGET_Z);
        rot2_matrix = rotate(-rev_theta, vec3(0.0f, 1.0f, 0.0f));
        model_matrix = trans3_matrix*rot2_matrix*trans2_matrix*scale_matrix*trans_matrix;
        draw_color_object(Sphere, SphereMagenta);

    }
}