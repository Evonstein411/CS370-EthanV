//
// Created by ethan on 11/18/2026.
//

#ifndef CS370_FA26_EDITORCONTROLLER_H
#define CS370_FA26_EDITORCONTROLLER_H

#include "../scene/scenemodel.h"
#include "hitdetection.h"

class EditorController {
public:
    explicit EditorController(SceneModel& model);

    // Called by Callbacks
    void handle_key(int key, int action, int mods);
    void handle_mouse_button(int button, int action, double x, double y);
    void handle_cursor(double x, double y);
    void handle_scroll(double xoffset, double yoffset);
    void handle_char(unsigned int codepoint);
    void handle_resize(int width, int height);

    // Commands
    void select_node(SceneNode* node);
    void clear_selection();
    void delete_selected();
    void toggle_camera_mode();
    void add_object();

private:
    SceneModel& model;
    // mouse look state
    bool rightMouseDown = false;
    double lastMouseX = 0.0;
    double lastMouseY = 0.0;
    float mouseLookSensitivity = 0.25f;

    // gizmo drag state
    GizmoAxis activeGizmo = GIZMO_NONE;
    vmath::vec3 dragStartPos;
    vmath::vec2 dragStartMouse;

    void handle_camera_keys(int key, int action);
    void handle_ortho_keys(int key, int action);
    void handle_perspective_keys(int key, int action);
    void handle_selection_click(double x, double y);
    void update_ortho_eye();
};




#endif //CS370_FA26_EDITORCONTROLLER_H
