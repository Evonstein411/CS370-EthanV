// CS370 - Exam03
// Fall 2025

/******************************************/
/*       INSERT (a) CODE HERE             */
/******************************************/
// Your name
//Ethan VonStein
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
void draw_table(GLuint t_obj, GLuint t_mat);

int main(int argc, char**argv)
{
    // Create OpenGL window
    GLFWwindow* window = CreateWindow("Exam 3 2026", ww, hh);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    } else {
        printf("OpenGL window successfully created\n");
    }
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
    // Create scene graph
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
        // Update angle based on time for fixed rpm
        GLdouble curTime = glfwGetTime();
        if (animate) {
            double dT = (curTime-elTime);
            flashTime += dT;        
            // Animate tree, skirt, and turkey
            theta += rpm*(360.0/60.0)*dT;
    
			/******************************************/
			/*       INSERT (p) CODE HERE             */
			/******************************************/
            // TODO: Adjust update transformations


            if (mode > 9) {
                /******************************************/
                /*       INSERT (t) CODE HERE (BONUS)     */
                /******************************************/
                // TODO: Flash star

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

    if (mode > 8) {
        draw_background(Background, Snow);
    }

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
    proj_matrix = ortho(-10.0f*xratio, 10.0f*xratio, -10.0f*yratio, 10.0f*yratio, -10.0f, 10.0f);

    // Set camera matrix
    camera_matrix = lookat(eye, center, up);

    // Render objects
    render_scene();

    glFlush();
}

void render_scene( ) {
    // Declare transformation matrices
    model_matrix = mat4().identity();
    mat4 scale_matrix = mat4().identity();
    mat4 rot_matrix = mat4().identity();
    mat4 trans_matrix = mat4().identity();
    draw_table(Cube, WhitePlastic);

    if (mode > 0) {
        /******************************************/
        /*       INSERT (c) CODE HERE             */
        /******************************************/
        // TODO: Draw carpet
        trans_matrix = translate(0.0f, 0.1f, 0.0f);
        scale_matrix = scale(4.0f, 1.0f, 4.0f);
        rot_matrix = rotate(-45.0f, vec3(0.0f, 1.0f, 0.0f));
        model_matrix = trans_matrix*scale_matrix*rot_matrix;
        draw_tex_object(Carpet, Grinch);
    }
    if (mode > 1) {
        /******************************************/
        /*       INSERT (h) CODE HERE             */
        /******************************************/
        // TODO: Traverse scene graph
        traverse_scene_graph(&base, mat4().identity());

    }
}

void draw_background(GLuint b_obj, GLuint b_tex){
    // Set anisotropic scaling
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
    // TODO: Set default orthographic projection
    proj_matrix = ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

    // TODO: Set default camera matrix
    camera_matrix = mat4().identity();

    // TODO: Set default model matrix
    model_matrix = mat4().identity();

    /******************************************/
    /*       INSERT (r) CODE HERE             */
    /******************************************/
    // TODO: Draw background

}

void draw_table(GLuint t_obj, GLuint t_mat){
    // Draw cube
    mat4 trans_matrix = translate(0.0f, -0.1f, 0.0f);
    mat4 scale_matrix = scale(TABLE_SIZE, 0.2f, TABLE_SIZE);
    model_matrix = trans_matrix*scale_matrix;
    normal_matrix = model_matrix.inverse().transpose();
    draw_mat_object(t_obj, t_mat);
}
