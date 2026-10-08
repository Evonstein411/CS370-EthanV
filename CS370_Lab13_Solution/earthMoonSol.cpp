#include <stdio.h>
#include <vector>
#include "../common/GLFWutils.h"
#include "../common/textureutils.h"
#include "../common/objloader.h"
#include "../common/vmath.h"
#include "Globals.h"
#include "geometry.h"
#include "textures.h"
#include "drawObjects.h"
#include "shaders.h"
#include "callbacks.h"

void display( );
void render_scene( );
void draw_background(GLuint b_obj, GLuint b_tex);

int main(int argc, char**argv)
{
	// Create OpenGL window
	GLFWwindow* window = CreateWindow("Earth Moon", ww, hh);
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
    // Create textures
    build_textures();
	// Create shaders
	build_shaders();

    // Enable depth test
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

	// Start loop
    while ( !glfwWindowShouldClose( window ) ) {
    	// Draw graphics
        display();
        // Update other events like input handling
        glfwPollEvents();
        // Update angle based on time for fixed rpm
        GLdouble curTime = glfwGetTime();
        if (animate) {
            earth_angle += (curTime - elTime) * (rpm / 60.0) * 360.0;
            moon_angle += (curTime - elTime) * (rpm / 60.0) * 360.0;
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

    // TODO: Render background
	draw_background(Background, Space);

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
    proj_matrix = frustum(-1.0f*xratio, 1.0f*xratio, -1.0f*yratio, 1.0f*yratio, 1.0f, 100.0f);

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
	mat4 rot2_matrix = mat4().identity();


    // Set earth transformation matrix
    trans_matrix = translation(0.0f, 0.0f, 0.0f);
    rot_matrix = rotation(earth_angle, normalize(axis));
    scale_matrix = scale(1.0f, 1.0f, 1.0f);
    model_matrix = trans_matrix*rot_matrix*scale_matrix;
    // TODO: Draw earth
    draw_tex_object(Sphere, Earth);

    // Set moon transformation matrix
    rot_matrix = rotation(moon_angle, normalize(axis));
    scale_matrix = scale(0.25f, 0.25f, 0.25f);
    trans_matrix = translation(2.0f, 0.0f, 0.0f);
    model_matrix = rot_matrix*trans_matrix*rot_matrix*scale_matrix;
    // TODO: Draw moon
    draw_tex_object(Sphere, Moon);
}

void draw_background(GLuint b_obj, GLuint b_texid){
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

    // TODO: Draw background with depth buffer writes disabled
    glDepthMask(GL_FALSE);
    draw_tex_object(b_obj, b_texid);
    glDepthMask(GL_TRUE);
}
