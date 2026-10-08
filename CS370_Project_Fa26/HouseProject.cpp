// CS370 Final Project
// Fall 2025

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
#include "hud.h"
#include "callbacks.h"

vector<Widget*> widgets;


void display();
void render_scene();
void render_hud();
void init_hud();


int main(int argc, char**argv)
{
	// Create OpenGL window
	GLFWwindow* window = CreateWindow("Think OUTSIDE The Box 2026", ww, hh);
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

    // Create HUD
    init_hud();

    // Enable depth test
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    // Set Initial camera position
    GLfloat x, y, z;
    x = (GLfloat)(radius*sin(deg2rad(azimuth))*sin(deg2rad(elevation)));
    y = (GLfloat)(radius*cos(deg2rad(elevation)));
    z = (GLfloat)(radius*cos(deg2rad(azimuth))*sin(deg2rad(elevation)));
    eye = vec3(x,y,z);

    // Start loop
    while ( !glfwWindowShouldClose( window ) ) {
        //update dynamic materials
        build_materials();
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
    proj_matrix = ortho(-5.0f*xratio, 5.0f*xratio, -5.0f*yratio, 5.0f*yratio, -5.0f, 5.0f);

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
    mat4 scale_matrix = mat4().identity();
    mat4 rot_matrix = mat4().identity();
    mat4 trans_matrix = mat4().identity();

    // Set cube transformation matrix
    trans_matrix = translate(0.0f, 0.0f, 0.0f);
    rot_matrix = rotate(0.0f, vec3(0.0f, 0.0f, 1.0f));
    scale_matrix = scale(2.0f, 2.0f, 2.0f);
	model_matrix = trans_matrix*rot_matrix*scale_matrix;
    normal_matrix = model_matrix.inverse().transpose();
    // Draw material obj
    draw_mat_object(Sphere, Brass);

    //floor
    trans_matrix = translate(0.0f, -3.0f, 0.0f);
    rot_matrix = rotate(0.0f, vec3(0.0f, 0.0f, 1.0f));
    scale_matrix = scale(20.0f, 2.0f, 20.0f);
    model_matrix = trans_matrix*rot_matrix*scale_matrix;
    draw_mat_object(Cube, FloorGray);

}


void render_hud() {
    //don't need to check depth for ui
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    //allow transparency (alpha blending)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //switch to pixel based ortho camera, store camera data pre hud draw
    mat4 scene_proj_matrix = proj_matrix;
    mat4 scene_camera_matrix = camera_matrix;
    proj_matrix = ortho(0.0f, (float)ww, 0.0f, (float)hh, -1.0f, 1.0f);
    camera_matrix  = mat4().identity();


    //model matrices
    model_matrix = mat4().identity();
    mat4 scale_matrix = mat4().identity();
    mat4 trans_matrix = mat4().identity();


    for (size_t i = 0; i < widgets.size(); i++) {
        widgets[i]->draw();
    }



    //reset settings to pre hud state, restore camera
    proj_matrix = scene_proj_matrix;
    camera_matrix = scene_camera_matrix;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDisable(GL_BLEND);

}


void toggle_lights() {
    lightOn[WhitePointLight] = !lightOn[WhitePointLight];
}


void init_hud() {
    Button* light_button = new Button(0.0f, 0.0f, 240.0f, 40.0f, toggle_lights, "TOGGLE LIGHTS");
    Label* material_label= new Label(20.0f, 70.0f, 130.0f, 48.0f, "Material");

    Label* ambient_label = new Label(20.0f, 140.0f, 130.0f, 48.0f, "ambient");
    TextField* ambient_r_field = new TextField(180.0f, 140.0f, 65.0f, 48.0f, "R:", 4.0f, true, &ambient_r);
    ambient_r_field->setText(to_string(Materials[Brass].ambient[0]));
    TextField* ambient_g_field = new TextField(320.0f, 140.0f, 65.0f, 48.0f, "G:", 4.0f, true, &ambient_g);
    ambient_g_field->setText(to_string(Materials[Brass].ambient[1]));
    TextField* ambient_b_field = new TextField(460.0f, 140.0f, 65.0f, 48.0f, "B:", 4.0f, true, &ambient_b);
    ambient_b_field->setText(to_string(Materials[Brass].ambient[2]));
    TextField* ambient_a_field =new TextField(600.0f, 140.0f, 65.0f, 48.0f, "A:", 4.0f, true, &ambient_a);
    ambient_a_field->setText(to_string(Materials[Brass].ambient[3]));

    Label* diffuse_label= new Label(20.0f, 220.0f, 130.0f, 48.0f, "diffuse");
    TextField* diffuse_r_field = new TextField(180.0f, 220.0f, 65.0f, 48.0f, "R:", 4.0f, true, &diffuse_r);
    diffuse_r_field->setText(to_string(Materials[Brass].diffuse[0]));
    TextField* diffuse_g_field = new TextField(320.0f, 220.0f, 65.0f, 48.0f, "G:", 4.0f, true, &diffuse_g);
    diffuse_g_field->setText(to_string(Materials[Brass].diffuse[1]));
    TextField* diffuse_b_field = new TextField(460.0f, 220.0f, 65.0f, 48.0f, "B:", 4.0f, true, &diffuse_b);
    diffuse_b_field->setText(to_string(Materials[Brass].diffuse[2]));
    TextField* diffuse_a_field =new TextField(600.0f, 220.0f, 65.0f, 48.0f, "A:", 4.0f, true, &diffuse_a);
    diffuse_a_field->setText(to_string(Materials[Brass].diffuse[3]));

    Label* specular_label = new Label(20.0f, 300.0f, 130.0f, 48.0f, "specular");
    TextField* specular_r_field = new TextField(180.0f, 300.0f, 65.0f, 48.0f, "R:", 4.0f, true, &specular_r);
    specular_r_field->setText(to_string(Materials[Brass].specular[0]));
    TextField* specular_g_field = new TextField(320.0f, 300.0f, 65.0f, 48.0f, "G:", 4.0f, true, &specular_g);
    specular_g_field->setText(to_string(Materials[Brass].specular[1]));
    TextField* specular_b_field = new TextField(460.0f, 300.0f, 65.0f, 48.0f, "B:", 4.0f, true, &specular_b);
    specular_b_field->setText(to_string(Materials[Brass].specular[2]));
    TextField* specular_a_field =new TextField(600.0f, 300.0f, 65.0f, 48.0f, "A:", 4.0f, true, &specular_a);
    specular_a_field->setText(to_string(Materials[Brass].specular[3]));

    TextField* shininess_field = new TextField(20.0f, 380.0f, 65.0f, 48.0f, "shininess:", 4.0f, true, &shininess);
    shininess_field->setText(to_string(Materials[Brass].shininess));


}
