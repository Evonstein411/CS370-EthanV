
//includes
#include "desertfox.h"
#include "scene/scenemodel.h"
#include "editor/editorui.h"
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

    // setup initial state
    callbacks.register_all(window, &controller);
    renderer.initialize(width, height);
    sceneModel.create_default_scene();

    return true;
}


void DesertFox::run() {
    while (!glfwWindowShouldClose(window)) {

        controller.update();
        renderer.render(sceneModel, width, height);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
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





