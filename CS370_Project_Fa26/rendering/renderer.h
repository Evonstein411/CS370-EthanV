//
// Created by ethan on 11/18/2026.
//

#ifndef CS370_FA26_RENDERER_H
#define CS370_FA26_RENDERER_H

#include "../scene/scenemodel.h"
#include "../common/GLFWutils.h"
#include "../nodes/meshcomponent.h"
#include "editor/editorui.h"


struct HitResult {
    SceneNode* node = nullptr;
    GizmoAxis gizmo = GizmoAxis::NONE;
};

class Renderer {
public:
    explicit Renderer(SceneModel& model);

    void initialize(int width, int height);
    void resize(int width, int height);
    void render(SceneModel& model, int width, int height);
    HitResult pick(SceneModel &model, float mx, float my);
    EditorUI& get_ui(){return editorUI;}

private:
    void draw_scene(const SceneModel& model);
    void draw_selection_outline(const SceneModel& model, const Camera& camera, int width, int height);
    void draw_translation_gizmo(const SceneModel& model, const Camera& camera, GizmoAxis gizmoAxis);
    void draw_mesh(const MeshComponent* mesh, const vmath::mat4& modelMatrix, Camera camera);
    void draw_picking_object(GLuint mesh, GLuint colorBuffer, vmath::mat4 projMatrix, vmath::mat4 cameraMatrix, vmath::mat4 modelMatrix);
    void draw_pick_arrow(GLuint pickColorBuf, vmath::mat4 projMatrix, vmath::mat4 cameraMatrix , vmath::mat4 modelMatrix, GizmoAxis id);
    void create_outline_fbo(int width, int height);
    vmath::vec4 index_to_color(int i);
    int color_to_index(unsigned char r, unsigned char g, unsigned char b);

    GLuint outlineFBO = 0;
    GLuint outlineMaskTex = 0;
    EditorUI editorUI;
};




#endif //CS370_FA26_RENDERER_H
