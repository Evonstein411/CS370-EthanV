//
// Created by ethan on 11/18/2026.
//

#ifndef CS370_FA26_DESERTFOX_H
#define CS370_FA26_DESERTFOX_H

#include "../common/GLFWutils.h"
#include "scene/scenemodel.h"
#include "rendering/renderer.h"
#include "editor/editorcontroller.h"
#include "editor/callbacks.h"

class DesertFox {
public:
    DesertFox() = default;
    ~DesertFox();

    bool initialize();
    void run();
    void shutdown();

private:
    GLFWwindow* window = nullptr;

    SceneModel sceneModel;
    Renderer renderer;
    EditorController controller{sceneModel};
    Callbacks callbacks;

    int width = 1640;
    int height = 960;


};



#endif //CS370_FA26_DESERTFOX_H
