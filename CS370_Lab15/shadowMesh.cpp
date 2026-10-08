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
#include "callbacks.h"
#include "debugShadows.h"

using namespace vmath;
using namespace std;

void display( );
void render_scene( );
void create_shadows(LightProperties Light, GLuint s_buff);

int main(int argc, char**argv)
{
	// Create OpenGL window
	GLFWwindow* window = CreateWindow("Shadow Mesh", 640, 480);
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
    // Create shadow map
    build_textures();
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

    // Start loop
    while ( !glfwWindowShouldClose( window ) ) {
        // TODO: Create shadow buffer (cull front faces)


        // Uncomment instead of display() to view shadow buffer for debugging
        //debugShadows(ShadowTex1);
    	// Draw graphics
    	display();
        // Update other events like input handling
        glfwPollEvents();
        GLdouble curTime = glfwGetTime();
        if (animate) {
            sphere_angle += (curTime - elTime) * (rpm / 60.0) * 360.0;
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
    
    // Reset default viewport
    glViewport(0, 0, ww, hh);

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
    proj_matrix = frustum(-1.0f*xratio, 1.0f*xratio, -1.0f*yratio, 1.0f*yratio, 1.0f, 100.0f);

    // Set camera matrix
    camera_matrix = lookat(eye, center, up);

    // Render objects
    render_scene();

    glFlush();
}

void render_scene() {
    model_matrix = mat4().identity();
    mat4 scale_matrix = mat4().identity();
    mat4 rot_matrix = mat4().identity();
    mat4 trans_matrix = mat4().identity();


    // Set cube transformation matrix
    trans_matrix = translate(0.0f, -0.1f, 0.0f);
    scale_matrix = scale(10.0f, 0.2f, 10.0f);
    model_matrix = trans_matrix*scale_matrix;
    if (!shadow) {
        // Set normal matrix for phong shadow shader
        normal_matrix = model_matrix.inverse().transpose();
    }
    // TODO: Draw cube


    // Set sphere transformation matrix
    trans_matrix = translate(1.0f, 2.0f, 1.0f);
    rot_matrix = rotate(sphere_angle, vec3(0.0f, 1.0f, 0.0f));
    scale_matrix = scale(0.5f, 0.5f, 0.5f);
    model_matrix = rot_matrix*trans_matrix*scale_matrix;
    if (!shadow) {
        // Set normal matrix for phong shadow shader
        normal_matrix = model_matrix.inverse().transpose();
    }
    // TODO: Draw sphere


    // Set torus transformation matrix
    trans_matrix = translation(0.0f, 1.0f, 0.0f);
    rot_matrix = rotation(0.0f, 0.0f, 0.0f, 1.0f);
    scale_matrix = scale(1.5f, 0.5f, 1.5f);
    model_matrix = trans_matrix*rot_matrix*scale_matrix;
    if (!shadow) {
        // Set normal matrix for phong shadow shader
        normal_matrix = model_matrix.inverse().transpose();
    }
    // TODO: Draw torus


    // Draw sphere for light position (without shadow)
    if (!shadow) {
        trans_matrix = translate(Lights[0].position[0], Lights[0].position[1], Lights[0].position[2]);
        scale_matrix = scale(0.1f, 0.1f, 0.1f);
        model_matrix = trans_matrix * scale_matrix;
        // Set normal matrix for lighting shader
        normal_matrix = model_matrix.inverse().transpose();
        // Draw sphere
        draw_mat_shadow_object(Sphere, Brass);
    }

}

void create_shadows(LightProperties Light, GLuint s_buff){
    // TODO: Set shadow projection matrix


    // TODO: Set shadow camera matrix based on light position and direction


    // Change viewport to match shadow framebuffer size
    glViewport(0, 0, 1024, 1024);
    glBindFramebuffer(GL_FRAMEBUFFER, ShadowBufferIDs[s_buff]);
    glClear(GL_DEPTH_BUFFER_BIT);
    // TODO: Render shadow scene


    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Reset viewport
    glViewport(0, 0, ww, hh);
}
