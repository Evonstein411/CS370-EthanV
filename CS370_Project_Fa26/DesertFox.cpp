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
#include "scene.h"
#include "hitdetection.h"
#include "ui.h"
#include "callbacks.h"


//globals
vector<SceneObject*> objects;
SceneObject* selectedObject = nullptr;
GLuint outlineFBO = 0;
GLuint outlineMaskTex = 0;



//functions
void display();
void render_scene();
void init_scene();
void render_hud();
void createOutlineFBO(int width, int height);
void drawTranslationGizmo();
void drawSelectionOutline();




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
    // Create FBO for outlines
    createOutlineFBO(ww, hh);
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

    if (camera_mode == PERSPECTIVE) {
        float yaw = deg2rad(fpsAzimuth);
        center = eye + vec3(cos(yaw), 0.0f, sin(yaw));  // level at start
    }

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

    if (camera_mode == PERSPECTIVE) {
        float aspect = (float)ww / (float)hh;
        proj_matrix = perspective(60.0f, aspect, 0.1f, 200.0f);

        // Recompute center from current eye + angles (course method)
        float yaw   = deg2rad(fpsAzimuth);
        float pitch = deg2rad(fpsElevation);
        vec3 dir = normalize(vec3(
            cos(yaw) * cos(pitch),
            sin(pitch),
            sin(yaw) * cos(pitch)
        ));
        center = eye + dir;

        camera_matrix = lookat(eye, center, up);
    } else {
        // existing orbit + ortho
        proj_matrix = ortho(-radius * xratio, radius * xratio,
                            -radius * yratio, radius * yratio, -100.0f, 100.0f);
        camera_matrix = lookat(eye, center, up);
    }

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

    drawSelectionOutline();
    drawTranslationGizmo();


}

void init_scene() {
    int cube_id = find_mesh("cube");
    int sphere_id = find_mesh("sphere");
    if (cube_id < 0 || sphere_id < 0) {
        fprintf(stderr, "Required models missing from ../models\n");
        return;
    }


    SceneObject* sphereObject = new SceneObject(sphere_id, "sphere", {0.0f, 0.0f, 0.0f}, {2.0f, 2.0f, 2.0f}, {0.0f, 0.0f, 0.0f}, baseMaterial);
    SceneObject* floor = new SceneObject(cube_id, "floor", {0.0f, -3.0f, 0.0f}, {50.0f, 2.0f, 50.0f}, {0.0f, 0.0f, 0.0f}, baseMaterial);
    SceneObject* wall = new SceneObject(cube_id, "wall1", {25.0f, 3.0f, 00.0f}, {1.0f, 10.0f, 50.0f}, {0.0f, 0.0f, 0.0f}, baseMaterial);
    SceneObject* wall2 = new SceneObject(cube_id, "wall2", {-25.0f, 3.0f, 0.0f}, {1.0f, 10.0f, 50.0f}, {0.0f, 0.0f, 0.0f}, baseMaterial);
    SceneObject* wall3 = new SceneObject(cube_id, "wall3", {0.0f, 3.0f, 25.0f}, {50.0f, 10.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, baseMaterial);
    SceneObject* wall4 = new SceneObject(cube_id, "wall4", {0.0f, 3.0f, -25.0f}, {50.0f, 10.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, baseMaterial);
}


void createOutlineFBO(int width, int height) {
    if (outlineFBO) {
        glDeleteFramebuffers(1, &outlineFBO);
        glDeleteTextures(1, &outlineMaskTex);
    }
    glGenFramebuffers(1, &outlineFBO);
    glGenTextures(1, &outlineMaskTex);

    glBindTexture(GL_TEXTURE_2D, outlineMaskTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, outlineFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outlineMaskTex, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void drawSelectionOutline() {
    if (!selectedObject || selectedObject->mesh == (GLuint)-1) return;

    // Build model matrix the same way SceneObject::draw does
    mat4 trans = translate(selectedObject->position);
    mat4 rotX  = vmath::rotate(selectedObject->rotation[0], vec3(1,0,0));
    mat4 rotY  = vmath::rotate(selectedObject->rotation[1], vec3(0,1,0));
    mat4 rotZ  = vmath::rotate(selectedObject->rotation[2], vec3(0,0,1));
    mat4 scale_matrix = vmath::scale(selectedObject->scale);
    model_matrix = trans * rotX * rotY * rotZ * scale_matrix;

    // --- Mask pass ---
    glBindFramebuffer(GL_FRAMEBUFFER, outlineFBO);
    glViewport(0, 0, ww, hh);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    glUseProgram(mask_program);
    glUniformMatrix4fv(mask_proj_mat_loc,  1, GL_FALSE, proj_matrix);
    glUniformMatrix4fv(mask_cam_mat_loc,   1, GL_FALSE, camera_matrix);
    glUniformMatrix4fv(mask_model_mat_loc, 1, GL_FALSE, model_matrix);

    glBindVertexArray(VAOs[selectedObject->mesh]);
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[selectedObject->mesh][PosBuffer]);
    glVertexAttribPointer(mask_vPos, posCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(mask_vPos);
    glDrawArrays(GL_TRIANGLES, 0, numVertices[selectedObject->mesh]);

    // --- Edge pass (back to window) ---
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, ww, hh);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    glUseProgram(outline_program);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, outlineMaskTex);
    glUniform2f(outline_texel_size_loc, 1.0f / ww, 1.0f / hh);
    glUniform4f(outline_color_loc, 0.85f, 0.55f, 0.25f, 1.0f);
    glUniform1f(outline_thickness_loc, 1.0f);

    // Draw fullscreen quad
    glBindVertexArray(fullscreenVAO);

    glBindBuffer(GL_ARRAY_BUFFER, fullscreenPosBuffer);
    glVertexAttribPointer(outline_vPos, posCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(outline_vPos);

    glBindBuffer(GL_ARRAY_BUFFER, fullscreenTexBuffer);
    glVertexAttribPointer(outline_vTex, texCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(outline_vTex);

    glDrawArrays(GL_TRIANGLES, 0, fullscreenNumVertices);

    glEnable(GL_DEPTH_TEST);
}

void drawTranslationGizmo() {
    if (!selectedObject || gizmoArrowVAO == 0) return;

    float size = 0.35f;
    float length = 1.0f;
    float offset = length*2.0f;

    mat4 trans = translate(selectedObject->position);
    mat4 trans_offset = translate(0.0f, offset, 0.0f);
    mat4 scale = vmath::scale(size, length, size);
    mat4 rotate;

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    vec4 xColor = (activeGizmo == GIZMO_X) ? vec4(1.0f, 0.6f, 0.6f, 1.0f) : vec4(1.0f, 0.15f, 0.15f, 1.0f);
    vec4 yColor = (activeGizmo == GIZMO_Y) ? vec4(0.6f, 1.0f, 0.6f, 1.0f) : vec4(0.15f, 1.0f, 0.15f, 1.0f);
    vec4 zColor = (activeGizmo == GIZMO_Z) ? vec4(0.6f, 0.7f, 1.0f, 1.0f) : vec4(0.2f, 0.4f, 1.0f, 1.0f);

    // X (red) — rotate +Y onto +X, then scale
    rotate = vmath::rotate(90.0f, vec3(0,0,1));
    model_matrix = trans * rotate * trans_offset * scale;
    drawGizmoArrow(xColor);

    // Y (green)
    model_matrix = trans * trans_offset * scale;
    drawGizmoArrow(yColor);

    // Z (blue) — rotate +Y onto +Z
    rotate = vmath::rotate(-90.0f, vec3(1,0,0));
    model_matrix = trans * rotate * trans_offset * scale;
    drawGizmoArrow(zColor);

    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
}




