//
// Created by ethan on 11/18/2026.
//

#ifndef CS370_FA26_RENDERER_H
#define CS370_FA26_RENDERER_H

#include "../scene/scenemodel.h"
#include "../common/GLFWutils.h"
#include "../nodes/meshcomponent.h"
#include "editor/editorui.h"

enum class GizmoAxis { NONE = 0, X = 1001, Y = 1002, Z = 1003 };

struct HitResult {
    SceneNode* node = nullptr;
    GizmoAxis gizmo = GizmoAxis::NONE;
};

class Renderer {
public:
    Renderer() = default;

    void initialize(int width, int height);
    void resize(int width, int height);
    void render(SceneModel& model, int width, int height);
    HitResult pick(const SceneModel& model, float mx, float my);
    EditorUI& get_ui(){return editorUI;}

private:
    void draw_scene(const SceneModel& model);
    void draw_selection_outline(const SceneModel& model, const Camera& camera, int width, int height);
    void draw_translation_gizmo(const SceneModel& model, const Camera& camera, GizmoAxis gizmoAxis);
    void draw_mesh(const MeshComponent* mesh, const vmath::mat4& modelMatrix, Camera camera);
    void create_outline_fbo(int width, int height);
    static vmath::vec4 index_to_color(int i);
    static int color_to_index(unsigned char r, unsigned char g, unsigned char b);

    GLuint outlineFBO = 0;
    GLuint outlineMaskTex = 0;
    GLuint colorProgram = 0;
    GLint colorProjMatLoc = -1;
    GLint colorCamMatLoc = -1;
    GLint colorModelMatLoc = -1;
    GLuint colorVPos = 0;
    GLuint colorVCol = 0;


    EditorUI editorUI;
};




#endif //CS370_FA26_RENDERER_H
