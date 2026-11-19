//
// Created by ethan on 11/18/2026.
//

#ifndef CS370_FA26_RENDERER_H
#define CS370_FA26_RENDERER_H

#include "../scene/scenemodel.h"
#include "../common/GLFWutils.h"
#include "../nodes/meshcomponent.h"

class Renderer {
public:
    Renderer() = default;

    void initialize(int width, int height);
    void resize(int width, int height);
    void render(SceneModel& model, int width, int height);

private:
    void draw_scene(const SceneModel& model);
    void draw_selection_outline(const SceneModel& model, const Camera& camera, int width, int height);
    void draw_translation_gizmo(const SceneModel& model, const Camera& camera);
    void draw_mesh(const MeshComponent* mesh, const vmath::mat4& modelMatrix, Camera camera);
    void create_outline_fbo(int width, int height);

    GLuint outlineFBO = 0;
    GLuint outlineMaskTex = 0;
};




#endif //CS370_FA26_RENDERER_H
