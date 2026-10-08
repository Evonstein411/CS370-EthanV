// CS370 Assignment 1 - Don Quixote
// Fall 2026

// Your name
// Ethan VonStein
// -------------------------------------------------
// DOCUMENT YOUR UI CONTROLS AND ANY CREATIVITY HERE
//added day/night cycle to animation (works with reverse direction)
//includes sun moving across sky, switching to a moon at night, starts in the sky at night, different sky for day night and dawm/dusk
//made trapezoid geometry for roof
//made base for windmill and the fan
// -------------------------------------------------


#include <stdio.h>
#include <vector>
#include "../common/GLFWutils.h"
#include "../common/vmath.h"
#include "Globals.h"
#include "geometry.h"
#include "drawObjects.h"
#include "shaders.h"
#include "callbacks.h"

void display( );
void render_scene( );
void place_stars();

GLuint sunColor = SunYellow;
GLuint skyColor = SkyBlue;

int main(int argc, char**argv)
{
	// Create OpenGL window
	GLFWwindow* window = CreateWindow("Don Quixote 2026", ww, hh);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    } else {
        printf("OpenGL window successfully created\n");
    }

    // TODO: Register callbacks
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_callback);


    // Get initial time
    elTime = glfwGetTime();

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

        //  TODO: Update angle based on time for fixed rpm when animating
    	GLdouble curTime = glfwGetTime();
		GLdouble dt = curTime - elTime;

    	float theta = sun_angle * (M_PI / 180.0);
    	sun_x = -0.85f * cosf(theta);          // -0.85 ... 0 ... +0.85
    	sun_y = -0.15f + 0.85f * sinf(theta);  // ~horizon, peaks near 0.70

    	if(animate) {
    		fan_angle += dir*dt*(rpm/60.0)*360.0;
    		sun_angle += dt * sun_deg_per_sec*dir;
    		if(sun_angle < 0) { //avoid negative angles to simplify day/night shift logic
    			sun_angle = 360;
    		}else if (sun_angle > 360){
    		sun_angle = 0;
    		}


    		//change time
    		if( sun_angle > 200.0 && sun_angle < 240.0) {
    			sun_angle = 340.0;
    			isDay = !isDay;
    			if(!isDay) {place_stars();}
    		}else if(sun_angle < 340 && sun_angle > 300) {
    			sun_angle = 200;
    			isDay = !isDay;
    			if(!isDay) {place_stars();}
    		}

    		if(isDay) {
    			sunColor = SunYellow;
    			skyColor = SkyBlue;
    			//sunrise
    			if(sun_angle > 340 || sun_angle < 10) {
    				skyColor = SunsetSky;
    			}
    			//sunset
    			if(sun_angle > 170 && sun_angle < 200) {
    				skyColor = SunsetSky;
    			}
    		}else {
    			sunColor = MoonWhite;
    			skyColor = NightSky;
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
    model_matrix = mat4().identity();

    // TODO: Declare transformation matrices
	mat4 scale_matrix = mat4().identity();
	mat4 rot_matrix = mat4().identity();
	mat4 trans_matrix = mat4().identity();
	mat4 trans2_matrix = mat4().identity();

	// trans_matrix = translate(0.0f, 0.0f, 0.0f);
	// rot_matrix = rotate(0.0f, 0.0f, 0.0f, 1.0f);
	// scale_matrix = scale(1.0f, 1.0f, 1.0f);
	// model_matrix = trans_matrix*rot_matrix*scale_matrix;

    // TODO: Draw sky
	draw_color_object(Square, skyColor);
	if(!isDay) {
		for (int i = 0; i < NUM_STARS; i++) {
			scale_matrix = scale(star_size[i], star_size[i], 0.0f);
			trans_matrix = translate(star_pos[i][0], star_pos[i][1], 0.0f);
			model_matrix = trans_matrix * scale_matrix;
			draw_color_fan_object(Sun, MoonWhite);   // or FanBase / a StarWhite buffer
		}
	}

	// TODO: Draw sun (using draw_color_fan_object)
	scale_matrix = scale(0.10f, 0.10f, 0.0f);
	trans_matrix = translate(sun_x, sun_y, 0.0f);
	model_matrix = trans_matrix * scale_matrix;
	draw_color_fan_object(Sun, sunColor);


    // TODO: Draw grass
	scale_matrix = scale(1.0f, 0.4f, 1.0f);
	trans_matrix = translate(0.0f, -1.6f, 0.0f);
	model_matrix = scale_matrix*trans_matrix;
	draw_color_object(Square, GrassGreen);


    // TODO: Draw house

	scale_matrix = scale(0.25f, 0.5f, 0.0f);
	trans_matrix = translate(0.0f, 0.0f, 0.0f);
	model_matrix = scale_matrix*trans_matrix;
	draw_color_object(Square, HouseBrown);

	//base
	scale_matrix = scale(0.25f, 0.30f, 0.0f);
	trans_matrix = translate(0.0f, -1.5f, 0.0f);
	model_matrix = scale_matrix*trans_matrix;
	draw_color_object(Square, StoneBase);

	//base sides

	//right side
	scale_matrix = scale(0.05f, 0.30f, 0.0f);
	rot_matrix = rotate(90.0f, 0.0f, 0.0f, 1.0f);
	trans_matrix = translate(-1.5f, -6.0f, 0.0f);
	model_matrix = scale_matrix*rot_matrix*trans_matrix;
	draw_color_object(Triangle, StoneSide);
	//left side
	scale_matrix = scale(0.05f, 0.30f, 0.0f);
	rot_matrix = rotate(180.0f, 0.0f, 0.0f, 1.0f);
	trans_matrix = translate(6.0f, 1.5f, 0.0f);
	model_matrix = scale_matrix*rot_matrix*trans_matrix;
	draw_color_object(Triangle, StoneSide);

    // TODO: Draw roof
	scale_matrix = scale(0.25f, 0.15f, 0.0f);
	trans_matrix = translate(0.0f, 4.30f, 0.0f);
	model_matrix = scale_matrix*trans_matrix;
	draw_color_object(Trapezoid, RoofRed);


    // TODO: Draw fan
	//base
	scale_matrix = scale(0.025f, 0.025f, 0.0f);
	trans_matrix = translate(0.0f, 0.6f, 0.0f);
	model_matrix = trans_matrix*scale_matrix;
	draw_color_fan_object(Sun, FanBase);


	//fans
	scale_matrix = scale(0.3f, 0.1f, 0.0f);
	trans_matrix = translate(-1.0f, -1.0f, 0.0f);
	rot_matrix = rotate(0.0f+ fan_angle, 0.0f, 0.0f, 1.0f);
	trans2_matrix = translate(0.0f, 0.6f, 0.0f);
	model_matrix = trans2_matrix*rot_matrix*scale_matrix*trans_matrix;
	draw_color_object(Triangle, FanBlue);

	scale_matrix = scale(0.3f, 0.1f, 0.0f);
	trans_matrix = translate(-1.0f, -1.0f, 0.0f);
	rot_matrix = rotate(120.0f+ fan_angle, 0.0f, 0.0f, 1.0f);
	trans2_matrix = translate(0.0f, 0.6f, 0.0f);
	model_matrix = trans2_matrix*rot_matrix*scale_matrix*trans_matrix;
	draw_color_object(Triangle, FanBlue);


	scale_matrix = scale(0.3f, 0.1f, 0.0f);
	trans_matrix = translate(-1.0f, -1.0f, 0.0f);
	rot_matrix = rotate(240.0f+ fan_angle, 0.0f, 0.0f, 1.0f);
	trans2_matrix = translate(0.0f, 0.6f, 0.0f);
	model_matrix = trans2_matrix*rot_matrix*scale_matrix*trans_matrix;
	draw_color_object(Triangle, FanBlue);
}

void place_stars() {
	for (int i = 0; i < NUM_STARS; i++) {
		float x = (rand() / (float)RAND_MAX) * 2.0f - 1.0f;          // -1 .. 1
		float y = (rand() / (float)RAND_MAX) * 1.2f - 0.15f;         // -0.15 .. 1.05  (sky band)
		star_pos[i]  = vec2(x, y);
		star_size[i] = 0.003f + (rand() / (float)RAND_MAX) * 0.006f; // tiny
	}
}
