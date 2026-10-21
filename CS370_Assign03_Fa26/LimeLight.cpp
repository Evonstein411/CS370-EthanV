// CS370 Assignment 3 - Limelight
// Fall 2026


// Your name
//Ethan VonStein
// -------------------------------------------------
// DOCUMENT YOUR UI CONTROLS AND ANY CREATIVITY HERE
// added a torus with its own material, spot light, and animation (animate on off via 'M' turn light on and off via '4')
// added an global animate toggle binded to spacebar
// -------------------------------------------------


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



//ambient = background illimuniation
//diffuse = scattered light "roughness"
//specular = highlights "mirrorness"
//shininess = degree of concentration of specular

void display( );
void render_scene( );

int main(int argc, char**argv)
{
	// Create OpenGL window
	GLFWwindow* window = CreateWindow("Limelight 2026", ww, hh);
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
	// Create shaders
	build_shaders();

    // Enable depth test
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    // TODO: Enable alpha blending and set blend factors
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set background color
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);

    // Set Initial camera position
	GLfloat x, y, z;
	x = (GLfloat)(radius*sin(deg2rad(azimuth))*sin(deg2rad(elevation)));
	y = (GLfloat)(radius*cos(deg2rad(elevation)));
	z = (GLfloat)(radius*cos(deg2rad(azimuth))*sin(deg2rad(elevation)));
	eye = vec3(x,y,z);

    // Get initial time
    elTime = glfwGetTime();

	// Start loop
    while ( !glfwWindowShouldClose( window ) ) {
    	// Draw graphics
        display();
        // Update other events like input handling
        glfwPollEvents();
        GLdouble curTime = glfwGetTime();
    	GLdouble deltaTime = curTime - elTime;
		// TODO: Add animations
    	if (cube_slide) {
    		cube_pos[2] += cube_dir * cube_slps * deltaTime;
    		if (fabs(cube_pos[2]) > 2.0f) {
    			cube_dir *= -1.0f;
    		}
    	}

    	if (sphere_bounce) {
    		sphere_pos[1] += sphere_dir * sphere_bps * deltaTime;
    		if (sphere_pos[1] > 2.5f || sphere_pos[1] < 1.0f) {
    			sphere_dir *= -1.0f;
    		}
    	}

    	if (pyr_spin) {
    		pyr_ang += pyr_dps * deltaTime;
    		if (pyr_ang > 360.0f) {
    			pyr_ang -= 360.0f;
    		}
    	}

    	if(torus_animate) {
    		torus_ang += torus_dps * deltaTime;
    		if (torus_ang > 360.0f) {
    			torus_ang -= 360.0f;
    		}
    		torus_pos[1] += torus_dir * torus_bps * deltaTime;
    		if (torus_pos[1] > 2.5f || torus_pos[1] < 1.0f) {
    			torus_dir *= -1.0f;
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
    proj_matrix = ortho(-10.0f*xratio, 10.0f*xratio, -10.0f*yratio, 10.0f*yratio, -10.0f, 10.0f);

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

	// TODO: Draw objects

	//torus
	trans_matrix = translate(torus_pos);
	scale_matrix = scale(1.0f, 1.0f, 1.0f);
	rot_matrix = rotate(torus_ang, vec3(0.0f, 0.0f, 1.0f));
	model_matrix = trans_matrix * rot_matrix * scale_matrix;
	normal_matrix = model_matrix.inverse().transpose();
	draw_mat_object(Torus, TorusMaterial);


	//sphere
	trans_matrix = translate(sphere_pos);
	scale_matrix = scale(1.0f, 1.0f, 1.0f);
	model_matrix = trans_matrix*scale_matrix;
	normal_matrix = model_matrix.inverse().transpose();
	draw_mat_object(Sphere, SphereMaterial);

	//cube
	trans_matrix = translate(cube_pos);
	scale_matrix = scale(1.0f, 1.0f, 1.0f);
	rot_matrix = rotate(0.0f, vec3(0.0f, 0.0f, 1.0f));
	model_matrix = trans_matrix * rot_matrix * scale_matrix;
	normal_matrix = model_matrix.inverse().transpose();
	draw_mat_object(Cube, CubeMaterial);


	//oct
	trans_matrix = translate(pyr_pos);
	scale_matrix = scale(1.0f, 1.0f, 1.0f);
	rot_matrix = rotate(pyr_ang, vec3(0.0f, 1.0f, 0.0f));
	model_matrix = trans_matrix * rot_matrix * scale_matrix;
	normal_matrix = model_matrix.inverse().transpose();
	draw_mat_object(Octahedron, OctMaterial);

	//stage
	trans_matrix = translate(0.0f, -0.5f, 0.0f);
	scale_matrix = scale(15.0f, 1.0f, 7.5f);
	model_matrix = trans_matrix*scale_matrix;
	normal_matrix = model_matrix.inverse().transpose();
	draw_mat_object(Cube, Wood);

	//stage cylinder
	trans_matrix = translate(0.0f, -0.5f, 3.75f);
	scale_matrix = scale(7.5f, 0.5f, 7.5f);
	rot_matrix = rotate(0.0f, vec3(0.0f, 0.0f, 1.0f));
	model_matrix = trans_matrix * rot_matrix * scale_matrix;
	normal_matrix = model_matrix.inverse().transpose();
	draw_mat_object(Cylinder, Wood);

	//half circle
	trans_matrix = translate(0.0f, 0.0f, 3.75f);
	scale_matrix = scale(5.25f, 0.5f, 5.25f);
	rot_matrix = rotate(180.0f, vec3(0.0f, 1.0f, 0.0f));
	model_matrix = trans_matrix * rot_matrix * scale_matrix;
	normal_matrix = model_matrix.inverse().transpose();
	draw_mat_object(HalfCircle, Wood);
}
