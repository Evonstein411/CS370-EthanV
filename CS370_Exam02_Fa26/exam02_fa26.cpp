// CS370 - Exam 2
// Fall 2026

/******************************************/
/*       INSERT (a) CODE HERE             */
/******************************************/
// Your name
//Ethan VonStein
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
void draw_table();

int main(int argc, char**argv)
{
	// Create OpenGL window
	GLFWwindow* window = CreateWindow("Exam 2 2026", ww, hh);
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
    // Create material buffers
    build_materials();
    // Create light buffers
    build_lights();
	// Create shaders
	build_shaders();

    // Enable depth test
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    // Set background color
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    /******************************************/
    /*       INSERT (k) CODE HERE             */
    /******************************************/
    // TODO: Enable blending and set blend factors
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // Set initial camera position
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
        if (mode >= 6) {
            GLdouble dT = (curTime - elTime);
            lightOn[RedSpotLight] = 0;
            if (spinning) {
                /******************************************/
                /*       INSERT (i) CODE HERE             */
                /******************************************/
                plate_theta += spm * dT;
                lightOn[RedSpotLight] = 1;


            }
            // TODO: Time-based animation for pouring teapot
            lightOn[CyanSpotLight] = 0;
            if (tipping) {
                /******************************************/
                /*       INSERT (m/n) CODE HERE           */
                /******************************************/
                pot_theta += ppm * dT;
                lightOn[CyanSpotLight] = 1;
                if (pot_theta < pour_ang || pot_theta > 0) {

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

    // Set projection matrix
    proj_matrix = ortho(-8.0f*xratio, 8.0f*xratio, -8.0f*yratio, 8.0f*yratio, -8.0f, 8.0f);

    // Set camera matrix
    camera_matrix = lookat(eye, center, up);

    // Render objects
	render_scene();

	// Flush pipeline
	glFlush();
}

void render_scene( ) {
    // Declare transformation matrices
    model_matrix = mat4().identity();
    mat4 scale_matrix = mat4().identity();
    mat4 scale2_matrix = mat4().identity();
    mat4 rot_matrix = mat4().identity();
    mat4 rot2_matrix = mat4().identity();
    mat4 trans_matrix = mat4().identity();
    mat4 trans2_matrix = mat4().identity();

    // Mode 0/1: Table
    draw_table();


    // Mode 2: Brass plate
    if (mode == 2) {
        /******************************************/
        /*       INSERT (d) CODE HERE             */
        /******************************************/
        // TODO: Render copper plate
        trans_matrix = translate(PLATE_X, 0.0f, PLATE_Z);
        scale_matrix = scale(2.0f, 0.1f, 2.0f);
        model_matrix = trans_matrix * scale_matrix;
        normal_matrix = model_matrix.inverse().transpose();
        draw_mat_object(Plate, Copper);
    }
    // Mode 3: Copper plate and donut
    else if (mode==3)
    {
        /******************************************/
        /*       INSERT (f) CODE HERE             */
        /******************************************/
        // TODO: Render copper plate
        trans_matrix = translate(PLATE_X, 0.0f, PLATE_Z);
        scale_matrix = scale(2.0f, 0.1f, 2.0f);
        model_matrix = trans_matrix * scale_matrix;
        normal_matrix = model_matrix.inverse().transpose();
        draw_mat_object(Plate, Copper);

        // TODO: Render donut
        trans_matrix = translate(PLATE_X, 0.4f, PLATE_Z);
        scale_matrix = scale(0.5f, 0.5f, 0.5f);
        model_matrix = trans_matrix * scale_matrix;
        normal_matrix = model_matrix.inverse().transpose();
        draw_mat_object(Donut, Dough);

    }
    // Mode 4/5: Copper plate and stack of donuts
    else if (mode==4 || mode==5)
    {
        /******************************************/
        /*       INSERT (g) CODE HERE             */
        /******************************************/
        // TODO: Render copper plate
        trans_matrix = translate(PLATE_X, 0.0f, PLATE_Z);
        scale_matrix = scale(2.0f, 0.1f, 2.0f);
        model_matrix = trans_matrix * scale_matrix;
        normal_matrix = model_matrix.inverse().transpose();
        draw_mat_object(Plate, Copper);
        // TODO: Render donut stack

        trans_matrix = translate(PLATE_X + 0.5f, 0.4f, PLATE_Z + 0.75f);
        scale_matrix = scale(0.5f, 0.5f, 0.5f);
        model_matrix = trans_matrix * scale_matrix;
        normal_matrix = model_matrix.inverse().transpose();
        draw_mat_object(Donut, Dough);

        trans_matrix = translate(PLATE_X + 0.5f, 0.4f, PLATE_Z - 0.75f );
        scale_matrix = scale(0.5f, 0.5f, 0.5f);
        model_matrix = trans_matrix * scale_matrix;
        normal_matrix = model_matrix.inverse().transpose();
        draw_mat_object(Donut, Dough);

        trans_matrix = translate(PLATE_X - 0.75f, 0.4f, PLATE_Z - 0.0f );
        scale_matrix = scale(0.5f, 0.5f, 0.5f);
        model_matrix = trans_matrix * scale_matrix;
        normal_matrix = model_matrix.inverse().transpose();
        draw_mat_object(Donut, Dough);

        trans_matrix = translate(PLATE_X, 0.9f, PLATE_Z);
        scale_matrix = scale(0.5f, 0.5f, 0.5f);
        model_matrix = trans_matrix * scale_matrix;
        normal_matrix = model_matrix.inverse().transpose();
        draw_mat_object(Donut, Dough);

    }
    // Mode 6: Spinning copper plate and stack of donuts
    else if (mode==6) {
        /******************************************/
        /*       INSERT (i) CODE HERE             */
        /******************************************/
        // TODO: Render copper plate
        trans_matrix = translate(PLATE_X, 0.0f, PLATE_Z);
        scale_matrix = scale(2.0f, 0.1f, 2.0f);
        rot_matrix = rotate(plate_theta, vec3(0.0f, 1.0f, 0.0f));
        model_matrix = trans_matrix * scale_matrix * rot_matrix;
        normal_matrix = model_matrix.inverse().transpose();
        draw_mat_object(Plate, Copper);

        // TODO: Render donut stack

        trans_matrix = translate(PLATE_X, 0.4f, PLATE_Z);
        trans2_matrix = translate(0.5f, 0.0f, 0.75f);
        scale_matrix = scale(0.5f, 0.5f, 0.5f);
        rot_matrix = rotate(plate_theta, vec3(0.0f, 1.0f, 0.0f));
        model_matrix = trans_matrix * rot_matrix * trans2_matrix *  scale_matrix;
        normal_matrix = model_matrix.inverse().transpose();
        draw_mat_object(Donut, Dough);

        trans_matrix = translate(PLATE_X, 0.4f, PLATE_Z);
        trans2_matrix = translate(0.5f, 0.0f, -0.75f);
        scale_matrix = scale(0.5f, 0.5f, 0.5f);
        rot_matrix = rotate(plate_theta, vec3(0.0f, 1.0f, 0.0f));
        model_matrix = trans_matrix * rot_matrix * trans2_matrix *  scale_matrix;
        normal_matrix = model_matrix.inverse().transpose();
        draw_mat_object(Donut, Dough);

        trans_matrix = translate(PLATE_X, 0.4f, PLATE_Z);
        trans2_matrix = translate(-0.75f, 0.0f, 0.0f);
        scale_matrix = scale(0.5f, 0.5f, 0.5f);
        rot_matrix = rotate(plate_theta, vec3(0.0f, 1.0f, 0.0f));
        model_matrix = trans_matrix * rot_matrix * trans2_matrix *  scale_matrix;
        normal_matrix = model_matrix.inverse().transpose();
        draw_mat_object(Donut, Dough);

        trans_matrix = translate(PLATE_X, 0.4f, PLATE_Z);
        trans2_matrix = translate(0.0f, 0.5f, 0.0f);
        scale_matrix = scale(0.5f, 0.5f, 0.5f);
        rot_matrix = rotate(plate_theta, vec3(0.0f, 1.0f, 0.0f));
        model_matrix = trans_matrix * rot_matrix * trans2_matrix *  scale_matrix;
        normal_matrix = model_matrix.inverse().transpose();
        draw_mat_object(Donut, Dough);

    }
    // Mode 7: Copper plate and stack of donuts with teapot and mug
    else if (mode==7)
    {
        /******************************************/
        /*       INSERT (k) CODE HERE             */
        /******************************************/
// TODO: Render copper plate
        trans_matrix = translate(PLATE_X, 0.0f, PLATE_Z);
        scale_matrix = scale(2.0f, 0.1f, 2.0f);
        rot_matrix = rotate(plate_theta, vec3(0.0f, 1.0f, 0.0f));
        model_matrix = trans_matrix * scale_matrix * rot_matrix;
        normal_matrix = model_matrix.inverse().transpose();
        draw_mat_object(Plate, Copper);

        // TODO: Render donut stack

        trans_matrix = translate(PLATE_X, 0.4f, PLATE_Z);
        trans2_matrix = translate(0.5f, 0.0f, 0.75f);
        scale_matrix = scale(0.5f, 0.5f, 0.5f);
        rot_matrix = rotate(plate_theta, vec3(0.0f, 1.0f, 0.0f));
        model_matrix = trans_matrix * rot_matrix * trans2_matrix *  scale_matrix;
        normal_matrix = model_matrix.inverse().transpose();
        draw_mat_object(Donut, Dough);

        trans_matrix = translate(PLATE_X, 0.4f, PLATE_Z);
        trans2_matrix = translate(0.5f, 0.0f, -0.75f);
        scale_matrix = scale(0.5f, 0.5f, 0.5f);
        rot_matrix = rotate(plate_theta, vec3(0.0f, 1.0f, 0.0f));
        model_matrix = trans_matrix * rot_matrix * trans2_matrix *  scale_matrix;
        normal_matrix = model_matrix.inverse().transpose();
        draw_mat_object(Donut, Dough);

        trans_matrix = translate(PLATE_X, 0.4f, PLATE_Z);
        trans2_matrix = translate(-0.75f, 0.0f, 0.0f);
        scale_matrix = scale(0.5f, 0.5f, 0.5f);
        rot_matrix = rotate(plate_theta, vec3(0.0f, 1.0f, 0.0f));
        model_matrix = trans_matrix * rot_matrix * trans2_matrix *  scale_matrix;
        normal_matrix = model_matrix.inverse().transpose();
        draw_mat_object(Donut, Dough);

        trans_matrix = translate(PLATE_X, 0.4f, PLATE_Z);
        trans2_matrix = translate(0.0f, 0.5f, 0.0f);
        scale_matrix = scale(0.5f, 0.5f, 0.5f);
        rot_matrix = rotate(plate_theta, vec3(0.0f, 1.0f, 0.0f));
        model_matrix = trans_matrix * rot_matrix * trans2_matrix *  scale_matrix;
        normal_matrix = model_matrix.inverse().transpose();
        draw_mat_object(Donut, Dough);

        // TODO: Render teapot
        trans_matrix = translate(MUG_X - 1.4f, 1.2f, MUG_Z + 1.4f);
        scale_matrix = scale(1.0f, 1.0f, 1.0f);
        rot_matrix = rotate(45.0f, vec3(0.0f, 1.0f, 0.0f));
        model_matrix =  trans_matrix * rot_matrix *  scale_matrix;
        normal_matrix = model_matrix.inverse().transpose();
        draw_mat_object(Teapot, BlueAcrylic);


        // TODO: Render mug
        trans_matrix = translate(MUG_X, 0.0f, MUG_Z);
        scale_matrix = scale(0.5f, 0.5f, 0.5f);
        rot_matrix = rotate(180.0f, vec3(0.0f, 1.0f, 0.0f));
        model_matrix =   trans_matrix * rot_matrix * scale_matrix;
        normal_matrix = model_matrix.inverse().transpose();
        draw_mat_object(Mug, GreenGlass);
    }
    // Mode 8: Copper plate and stack of donuts with animated teapot and mug
    else if (mode==8)
    {
        /******************************************/
        /*       INSERT (m) CODE HERE             */
        /******************************************/



    }
    // Extra Credit Mode 9: Copper plate and stack of donuts with animated teapot and mug
    // filling with coffee
    else if (mode==9)
    {
        /******************************************/
        /*       INSERT (n) CODE HERE             */
        /******************************************/
        // TODO: Render copper plate

        // TODO: Render donut stack

        // TODO: Render teapot

        // TODO: Render mug

        // TODO: Render coffee

    }

}

void draw_table(){
    // Draw cube
    mat4 trans_matrix = translate(0.0f, -0.1f, 0.0f);
    mat4 scale_matrix = scale(TABLE_SIZE, 0.2f, TABLE_SIZE);
    model_matrix = trans_matrix*scale_matrix;
    normal_matrix = model_matrix.inverse().transpose();
    draw_mat_object(Cube, Steel);

    model_matrix = translate(0.0f, 0.12f, 0.0f);
    draw_axes(Axes, AxesColor);
}
