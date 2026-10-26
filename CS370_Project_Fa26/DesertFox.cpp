// CS370 Final Project
// Fall 2026 Ethan VonStein

//includes
#include <stdio.h>
#include <vector>
#include "../common/GLFWutils.h"
#include "../common/textureutils.h"
#include "../common/objloader.h"
#include "../common/tangentspace.h"
#include "../common/vmath.h"
#include "../common/lighting.h"
#include "Globals.h"
#include "geometry.h"
#include "lights.h"
#include "materials.h"
#include "textures.h"
#include "drawObjects.h"
#include "shaders.h"
#include "sceneobject.h"
#include "hitdetection.h"
#include "ui.h"
#include "callbacks.h"


//globals
vector<SceneObject*> objects;
SceneObject* selectedObject = nullptr;



//functions
void display();
void render_scene();
void init_scene();
void render_hud();





int main(int argc, char**argv)
{
	// Create OpenGL window
	GLFWwindow* window = CreateWindow("Desert Fox", ww, hh);
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
    glfwSetCursorPosCallback(window, cursor_callback);
    glfwSetCharCallback(window, char_callback);

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
    // Create scene
    init_scene();
    // Create HUD
    init_hud();
    // Enable depth test
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    //enable opacity
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);

    // Set Initial camera position
    GLfloat x, y, z;
    x = (GLfloat)(radius*sin(deg2rad(azimuth))*sin(deg2rad(elevation)));
    y = (GLfloat)(radius*cos(deg2rad(elevation)));
    z = (GLfloat)(radius*cos(deg2rad(azimuth))*sin(deg2rad(elevation)));
    eye = vec3(x,y,z);

    // Start loop
    while ( !glfwWindowShouldClose( window ) ) {
        //update selected
        update_from_fields();
        //update dynamic materials
        build_dynamic_materials();
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

    // DEFAULT ORTHOGRAPHIC PROJECTION
    proj_matrix = ortho(-radius*xratio, radius*xratio, -radius*yratio, radius*yratio, -100.0, 100.0);

    // Set camera matrix
    camera_matrix = lookat(eye, center, up);

    // Render objects
	render_scene();

    // Render HUD
    render_hud();

	// Flush pipeline
	glFlush();
}

void render_scene( ) {
    // Declare transformation matrices
    model_matrix = mat4().identity();
    normal_matrix = mat4().identity();

    for (size_t i = 0; i < objects.size(); i++) {
        objects[i]->draw();
    }


}

void init_scene() {
    SceneObject* sphereObject = new SceneObject(Sphere, "sphere", {0.0f, 0.0f, 0.0f}, {2.0f, 2.0f, 2.0f}, {0.0f, 0.0f, 0.0f}, baseMaterial);
    SceneObject* floor = new SceneObject(Cube, "floor", {0.0f, -3.0f, 0.0f}, {50.0f, 2.0f, 50.0f}, {0.0f, 0.0f, 0.0f}, floormat);
    SceneObject* wall = new SceneObject(Cube, "wall1", {25.0f, 3.0f, 00.0f}, {1.0f, 10.0f, 50.0f}, {0.0f, 0.0f, 0.0f}, floormat);
    SceneObject* wall2 = new SceneObject(Cube, "wall2", {-25.0f, 3.0f, 0.0f}, {1.0f, 10.0f, 50.0f}, {0.0f, 0.0f, 0.0f}, floormat);
    SceneObject* wall3 = new SceneObject(Cube, "wall3", {0.0f, 3.0f, 25.0f}, {50.0f, 10.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, floormat);
    SceneObject* wall4 = new SceneObject(Cube, "wall3", {0.0f, 3.0f, -25.0f}, {50.0f, 10.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, floormat);
}








