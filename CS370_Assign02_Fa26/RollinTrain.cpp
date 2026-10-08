// CS370 Assignment 2 - Rollin Train
// Fall 2026


// Your name
//Ethan VonStein
// -------------------------------------------------
// DOCUMENT YOUR UI CONTROLS AND ANY CREATIVITY HERE
// grass and dirt ground, blue sky box (culling on in ortho in order to draw sky nicely created inward facing cube for skybox)
//randomly placed trees
// -------------------------------------------------


#include <ctime>
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
void drawTrack(float x, float y, float z);
void drawBlocks(float x, float y, float z);
void drawTrain(float x, float y, float z);
void drawWheel(float x ,float y, float z);
void drawScenery(float x, float y, float z);
void drawTree(float x, float y, float z);
void plantTrees();

GLfloat trainX = 0.0f, trainY = 0.0f, trainZ = 0.0f;


int main(int argc, char**argv)
{
	// Create OpenGL window
	GLFWwindow* window = CreateWindow("Rollin Train 2026", ww, hh);
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
    plantTrees();
	// Create shaders
	build_shaders();

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Set background color
    glClearColor(1.0f,1.0f,1.0f,1.0f);

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
        GLdouble curTime = glfwGetTime();
        GLdouble dt = curTime - elTime;
        if (animate) {

            trainZ += dt*train_dir*speed;
            wheel_ang += train_dir*dt*(rpm/60.0)*360.0;
            smokeOffset += dt * smokeSpeed;
            if (smokeOffset > BODY_LENGTH/2) {
                smokeOffset = 0.0f;
            }

            if(trainZ <= -RAIL_LENGTH/2 + BODY_LENGTH/2 || trainZ >= RAIL_LENGTH/2 - BODY_LENGTH/2) {
                animate = false;
                train_dir = -train_dir;;
                //speed *= 1.25f;
            }









        }else {
            smokeOffset = 0.0f; //reset when animation ends
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

    // TODO: Set projection and camera matrices


    // Position camera for orthographic projection
    if (proj == ORTHOGRAPHIC)
    {
        glEnable(GL_CULL_FACE);

        GLfloat x, y, z;
        x = (GLfloat)(radius*sin(deg2rad(azimuth))*sin(deg2rad(elevation)));
        y = (GLfloat)(radius*cos(deg2rad(elevation)));
        z = (GLfloat)(radius*cos(deg2rad(azimuth))*sin(deg2rad(elevation)));

        eye = vec3(x,y,z);
        center = {0.0f, 0.0f, 0.0f};

        // Set orthographic (birds-eye) view (spherical coords)
        proj_matrix = ortho(-radius*xratio, radius*xratio, -radius*yratio, radius*yratio, -100.0, 100.0);
        camera_matrix = lookat(eye, center, up);

    }
    // Position camera for perspective projection
    else if (proj == PERSPECTIVE)
    {
        glDisable(GL_CULL_FACE);

        eye = vec3(trainX, trainY + BODY_HEIGHT + BODY_Y, trainZ + BODY_WIDTH);
        center = vec3(trainX, trainY + BODY_HEIGHT + BODY_Y, trainZ - RAIL_LENGTH);

        proj_matrix = frustum(-1.2f*xratio, 1.2f*xratio, -1.2f*yratio, 1.2f*yratio, 1.0f, 100.0f);
        camera_matrix = lookat(eye, center, up);


    }

    // Render objects
    render_scene();

    // draw axes
    //draw_axes(Axes, AxesColor);

    // Flush pipeline
    glFlush();
}

void render_scene( ) {
    // Declare transformation matrices
    model_matrix = mat4().identity();
    mat4 scale_matrix = mat4().identity();
    mat4 rot_matrix = mat4().identity();
    mat4 trans_matrix = mat4().identity();


    drawTrain(trainX, trainY, trainZ);
    drawTrack(0.0f, 0.0f, 0.0f);
    drawBlocks(0.0f, BOTTOM_BLOCK_SIZE/2, -RAIL_LENGTH/2 - BOTTOM_BLOCK_SIZE/2);
    drawScenery(0.0f, 0.0f, 0.0f);


    for (int i = 0; i < (int)treePositions.size(); i++) {
        drawTree(treePositions[i][0], treePositions[i][1], treePositions[i][2]);
    }




}


void drawTree(float x, float y, float z) {
    model_matrix = mat4().identity();
    mat4 scale_matrix = mat4().identity();
    mat4 rot_matrix = mat4().identity();
    mat4 trans_matrix = mat4().identity();


    trans_matrix = translate(x, y, z);
    scale_matrix = scale(TRUNK_RADIUS, TRUNK_HEIGHT, TRUNK_RADIUS);
    rot_matrix = rotate(0.0f, vec3(0.0f, 0.0f, 1.0f));
    model_matrix = trans_matrix * rot_matrix * scale_matrix;
    draw_color_object(Cylinder, WoodColor);

    trans_matrix = translate(x, y + LEAF_OFFSET_Y, z);
    scale_matrix = scale(LEAF_RADIUS, LEAF_HEIGHT, LEAF_RADIUS);
    rot_matrix = rotate(0.0f, vec3(0.0f, 0.0f, 1.0f));
    model_matrix = trans_matrix * rot_matrix * scale_matrix;
    draw_color_object(Cone, LeafColor);


}

void plantTrees() {
    srand((unsigned)time(NULL));
    treePositions.clear();

    const float grassHalfW = GRASS_WIDTH  * 0.5f;
    const float grassHalfL = GRASS_LENGTH * 0.5f;

    while ((int)treePositions.size() < TREE_COUNT) {
        float x = -grassHalfW + (rand() / (float)RAND_MAX) * GRASS_WIDTH;
        float z = -grassHalfL + (rand() / (float)RAND_MAX) * GRASS_LENGTH;
        treePositions.push_back(vec3(x, 0.0f, z));

    }
}

void drawScenery(float x, float y, float z) {
    model_matrix = mat4().identity();
    mat4 scale_matrix = mat4().identity();
    mat4 rot_matrix = mat4().identity();
    mat4 trans_matrix = mat4().identity();

    trans_matrix = translate(x, y + GRASS_OFFSET_Y, z);
    scale_matrix = scale(GRASS_WIDTH, GRASS_HEIGHT, GRASS_LENGTH);
    rot_matrix = rotate(0.0f, vec3(0.0f, 0.0f, 1.0f));
    model_matrix = trans_matrix * rot_matrix * scale_matrix;
    draw_color_object(Cube, GrassColor);

    trans_matrix = translate(x, y + DIRT_OFFSET_Y + 0.01f, z);
    scale_matrix = scale(DIRT_WIDTH, DIRT_HEIGHT, DIRT_LENGTH);
    rot_matrix = rotate(0.0f, vec3(0.0f, 0.0f, 1.0f));
    model_matrix = trans_matrix * rot_matrix * scale_matrix;
    draw_color_object(Cube, DirtColor);

    trans_matrix = translate(eye[0], eye[1], eye[2]);
    scale_matrix = scale(SKY_SIZE, SKY_SIZE, SKY_SIZE);
    rot_matrix = rotate(0.0f, vec3(0.0f, 0.0f, 1.0f));
    model_matrix = trans_matrix * rot_matrix * scale_matrix;

    draw_color_object(Skybox, SkyColor);


}

void drawTrack(float x, float y, float z) {
    // Declare transformation matrices
    model_matrix = mat4().identity();
    mat4 scale_matrix = mat4().identity();
    mat4 rot_matrix = mat4().identity();
    mat4 trans_matrix = mat4().identity();

    //rails
    trans_matrix = translate(x + RAIL_OFFSET_X, y + RAIL_OFFSET_Y, z +RAIL_OFFSET_Z);
    scale_matrix = scale(RAIL_WIDTH, RAIL_HEIGHT, RAIL_LENGTH);
    rot_matrix = rotate(0.0f, vec3(0.0f, 0.0f, 1.0f));
    model_matrix = trans_matrix * rot_matrix * scale_matrix;
    draw_color_object(Cube, RailColor);

    trans_matrix = translate(x - RAIL_OFFSET_X, y + RAIL_OFFSET_Y, z + RAIL_OFFSET_Z);
    scale_matrix = scale(RAIL_WIDTH, RAIL_HEIGHT, RAIL_LENGTH);
    rot_matrix = rotate(0.0f, vec3(0.0f, 0.0f, 1.0f));
    model_matrix = trans_matrix * rot_matrix * scale_matrix;
    draw_color_object(Cube, RailColor);


    //ties
    for (float i = -((RAIL_LENGTH-RAIL_TIES_LENGTH*4)/2) ; i < (RAIL_LENGTH/2) ; i += (RAIL_TIES_LENGTH*4)) {
        trans_matrix = translate(x + RAIL_TIES_OFFSET_X, y + RAIL_TIES_OFFSET_Y, z + i + RAIL_TIES_OFFSET_Z);
        scale_matrix = scale(RAIL_TIES_WIDTH, RAIL_TIES_HEIGHT, RAIL_TIES_LENGTH);
        rot_matrix = rotate(0.0f, vec3(0.0f, 0.0f, 1.0f));
        model_matrix = trans_matrix * rot_matrix * scale_matrix;
        draw_color_object(Cube, TiesColor);

    }

}


void drawBlocks(float x, float y, float z) {
    // Declare transformation matrices
    model_matrix = mat4().identity();
    mat4 scale_matrix = mat4().identity();
    mat4 rot_matrix = mat4().identity();
    mat4 trans_matrix = mat4().identity();

    trans_matrix = translate(x, y, z);
    scale_matrix = scale(BOTTOM_BLOCK_SIZE,  BOTTOM_BLOCK_SIZE, BOTTOM_BLOCK_SIZE);
    rot_matrix = rotate(0.0f, vec3(0.0f, 0.0f, 1.0f));
    model_matrix = trans_matrix * rot_matrix * scale_matrix;
    draw_color_object(Cube, BottomColor);

    trans_matrix = translate(x, y + BOTTOM_BLOCK_SIZE - MIDDLE_BLOCK_SIZE/2, z);
    scale_matrix = scale(MIDDLE_BLOCK_SIZE, MIDDLE_BLOCK_SIZE, MIDDLE_BLOCK_SIZE);
    rot_matrix = rotate(0.0f, vec3(0.0f, 0.0f, 1.0f));
    model_matrix = trans_matrix * rot_matrix * scale_matrix;
    draw_color_object(Cube, MiddleColor);

    trans_matrix = translate(x, y + BOTTOM_BLOCK_SIZE + MIDDLE_BLOCK_SIZE/2 -TOP_BLOCK_SIZE/2, z);
    scale_matrix = scale(TOP_BLOCK_SIZE, TOP_BLOCK_SIZE, TOP_BLOCK_SIZE);
    rot_matrix = rotate(0.0f, vec3(0.0f, 0.0f, 1.0f));
    model_matrix = trans_matrix * rot_matrix * scale_matrix;
    draw_color_object(Cube, TopColor);
}

void drawTrain(float x, float y, float z) {
    model_matrix = mat4().identity();
    mat4 scale_matrix = mat4().identity();
    mat4 rot_matrix = mat4().identity();
    mat4 trans_matrix = mat4().identity();
    //train
    //body
    trans_matrix = translate(x + BODY_X, y + BODY_Y, z + BODY_Z);
    scale_matrix = scale(BODY_WIDTH, BODY_HEIGHT, BODY_LENGTH);
    rot_matrix = rotate(0.0f, vec3(0.0f, 0.0f, 1.0f));
    model_matrix = trans_matrix * rot_matrix * scale_matrix;
    draw_color_object(Cube, BodyColor);
    //eng
    trans_matrix = translate(x + ENG_X, y+ENG_Y, z+ENG_Z);
    scale_matrix = scale(ENG_WIDTH, ENG_HEIGHT, ENG_LENGTH);
    rot_matrix = rotate(0.0f, vec3(0.0f, 0.0f, 1.0f));
    model_matrix = trans_matrix * rot_matrix * scale_matrix;
    draw_color_object(Cube, EngColor);
    //stack
    trans_matrix = translate(x + STACK_X, y + STACK_Y, z +STACK_Z);
    scale_matrix = scale(STACK_RADIUS, STACK_HEIGHT, STACK_RADIUS);
    rot_matrix = rotate(0.0f, vec3(0.0f, 0.0f, 1.0f));
    model_matrix = trans_matrix * rot_matrix * scale_matrix;
    draw_color_object(Cylinder, StackColor);
    //funnel
    trans_matrix = translate(x + STACK_X, y + STACK_Y, z + STACK_Z);
    scale_matrix = scale(FUNNEL_RADIUS, FUNNEL_HEIGHT, FUNNEL_RADIUS);
    rot_matrix = rotate(180.0f, vec3(0.0f, 0.0f, 1.0f));
    model_matrix = trans_matrix * rot_matrix * scale_matrix;
    draw_color_object(Cone, FunnelColor);


    //wheels
    drawWheel(x + WHEEL_X, y + WHEEL_Y, z + WHEEL_Z); //back r
    drawWheel(x-WHEEL_X, y+WHEEL_Y, z+WHEEL_Z); //back l

    drawWheel(x+WHEEL_X, y+WHEEL_Y, z+MID_WHEEL_OFFSET); //mid r
    drawWheel(x-WHEEL_X, y+WHEEL_Y, z+MID_WHEEL_OFFSET); //mid l

    drawWheel(x+WHEEL_X, y+WHEEL_Y, z-WHEEL_Z); //front r
    drawWheel(x-WHEEL_X, y+WHEEL_Y, z-WHEEL_Z); //front l

    //smoke
    if (animate) {
        trans_matrix = translate(x + (0.1f*smokeOffset), y + STACK_Y + FUNNEL_HEIGHT + smokeOffset, z + STACK_Z + smokeOffset);
        scale_matrix = scale(ENG_WIDTH, ENG_WIDTH, ENG_WIDTH);
        rot_matrix = rotate(0.0f, vec3(0.0f, 0.0f, 1.0f));
        model_matrix = trans_matrix * rot_matrix * scale_matrix;
        draw_color_object(Cube, RailColor);
    }



}


void drawWheel(float x ,float y, float z) {
    model_matrix = mat4().identity();
    mat4 scale_matrix = mat4().identity();
    mat4 rot_matrix = mat4().identity();
    mat4 rot2_matrix = mat4().identity();
    mat4 trans_matrix = mat4().identity();

    //tire
    trans_matrix = translate(x, y, z);
    scale_matrix = scale(WHEEL_RADIUS, WHEEL_WIDTH, WHEEL_RADIUS);
    rot_matrix = rotate(90.0f, vec3(0.0f, 0.0f, 1.0f));
    rot2_matrix = rotate(wheel_ang, vec3(0.0f, 1.0f, 0.0f));
    model_matrix = trans_matrix * rot_matrix * scale_matrix * rot2_matrix;
    draw_color_object(Torus, TireColor);

    //spokes
    for (int i = 0; i < 4; ++i) {
        trans_matrix = translate(x, y, z);
        scale_matrix = scale(SPOKE_WIDTH, SPOKE_WIDTH, SPOKE_LENGTH);
        rot_matrix = rotate(i * 45.0f + wheel_ang, vec3(1.0f, 0.0f, 0.0f));
        model_matrix = trans_matrix * rot_matrix * scale_matrix * rot2_matrix;
        draw_color_object(Cube, SpokeColor);
    }
}