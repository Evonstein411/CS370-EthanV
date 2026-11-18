
//includes
#include "desertfox.h"
#include <stdio.h>
#include <vector>
#include "../common/GLFWutils.h"
#include "../common/textureutils.h"
#include "../common/objloader.h"
#include "../common/tangentspace.h"
#include "../common/vmath.h"
#include "../common/lighting.h"
#include "Globals.h"
#include "rendering/geometry.h"
#include "rendering/lights.h"
#include "rendering/materials.h"
#include "rendering/textures.h"
#include "rendering/drawobjects.h"
#include "rendering/shaders.h"
#include "scene/scenemodel.h"
#include "editor/hitdetection.h"
#include "editor/ui.h"
#include "editor/callbacks.h"


bool DesertFox::initialize() {
    window = CreateWindow("Desert Fox", width, height);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return false;
    }

    // Store initial window size
    glfwGetFramebufferSize(window, &width, &height);

    // Register callbacks
    callbacks.register_all(window, &controller);

    // Create geometry buffers
    build_geometry();
    build_materials();
    build_lights();
    build_textures();
    build_shaders();

    renderer.initialize(width, height);
    sceneModel.create_default_scene();
    init_hud();


    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);

    return true;
}


void DesertFox::run() {
    while (!glfwWindowShouldClose(window)) {
        update_from_fields();          // UI → model sync (can move later)
        build_dynamic_materials();

        display();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}



void DesertFox::display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderer.render(sceneModel, width, height);
    render_hud();

    glFlush();
}


void DesertFox::shutdown() {
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    glfwTerminate();
}

DesertFox::~DesertFox() {
    shutdown();
}





