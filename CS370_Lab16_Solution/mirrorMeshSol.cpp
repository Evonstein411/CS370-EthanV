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
#include "debugTex.h"

using namespace vmath;
using namespace std;

void display( );
void render_scene( );
void create_mirror(GLuint m_texid);

int main(int argc, char**argv)
{
	// Create OpenGL window
	GLFWwindow* window = CreateWindow("Mirror Mesh", ww, hh);
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
    // Create textures (mirror)
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
        // TODO: Load environment map
        create_mirror(MirrorTex);
        // Uncomment instead of display() to view mirror map for debugging
        //debugTex(Mirror, MirrorTex);
    	// Draw graphics
    	display();
        // Update other events like input handling
        glfwPollEvents();
        GLdouble curTime = glfwGetTime();
        if (animate) {
            sphere_angle += (curTime-elTime)*(rpm/60.0)*360.0;
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
    proj_matrix = frustum(-1.0f*xratio, 1.0f*xratio, -1.0f*yratio, 1.0f*yratio, 1.0f, 100.0f);

    // Set camera matrix
    camera_matrix = lookat(eye, center, up);

    // Render objects
    render_scene( );

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
    // Compute normal matrix from model matrix
    normal_matrix = model_matrix.inverse().transpose();
    // Draw object
    draw_mat_object(Cube, Brass);
    
    // Set sphere transformation matrix
    trans_matrix = translate(1.0f, 2.0f, 1.0f);
    rot_matrix = rotate(sphere_angle, vec3(0.0f, 1.0f, 0.0f));
    scale_matrix = scale(0.5f, 0.5f, 0.5f);
    model_matrix = rot_matrix*trans_matrix*scale_matrix;
    // Compute normal matrix from model matrix
    normal_matrix = model_matrix.inverse().transpose();
    // Draw object
    draw_mat_object(Sphere, Brass);

    // Set torus transformation matrix
    trans_matrix = translation(0.0f, 1.0f, 0.0f);
    rot_matrix = rotation(0.0f, 0.0f, 0.0f, 1.0f);
    scale_matrix = scale(1.5f, 0.5f, 1.5f);
    model_matrix = trans_matrix*rot_matrix*scale_matrix;
    // Compute normal matrix from model matrix
    normal_matrix = model_matrix.inverse().transpose();
    // Draw object
    draw_mat_object(Torus, RedPlastic);

    // Draw mirror with wireframe
    if (!mirror) {
        draw_frame(Frame);
        // Render mirror in scene
        // TODO: Set mirror transformations
        trans_matrix = translate(mirror_eye);
        rot_matrix = rotate(-90.0f, vec3(1.0f, 0.0f, 0.0f));
        scale_matrix = scale(2.0f, 1.0f, 2.0f);
        model_matrix = trans_matrix * rot_matrix * scale_matrix;
        // TODO: Draw mirror
        draw_tex_object(Mirror, MirrorTex);
    }

}

void create_mirror(GLuint m_texid) {
    // Clear framebuffer for mirror rendering pass
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // TODO: Set mirror projection matrix
    proj_matrix = frustum(-0.2f, 0.2f, -0.2f, 0.2f, 0.2f, 100.0f);

    // TODO: Set mirror camera matrix
    camera_matrix = lookat(mirror_eye, mirror_center, mirror_up);

    // Render mirror scene (without mirror)
    mirror = true;
    render_scene();
    glFlush();
    mirror = false;

    // TODO: Activate texture unit 0
    glActiveTexture(GL_TEXTURE0);
    // TODO: Bind mirror texture
    glBindTexture(GL_TEXTURE_2D, TextureIDs[m_texid]);
    // TODO: Copy framebuffer into mirror texture
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, ww, hh, 0);
}
