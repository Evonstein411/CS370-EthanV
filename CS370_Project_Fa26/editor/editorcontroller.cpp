//
// Created by ethan on 11/18/2026.
//

#include "editorcontroller.h"
#include "hud.h"
#include "../common/GLFWutils.h"
#include "rendering/camera.h"


EditorController::EditorController(SceneModel& model, Renderer& view) : model(model), view(view) {}

void EditorController::update() {
    //nothing right now
}

void EditorController::handle_key(int key, int action, int mods) {
    if (HUD::instance().key(key, action)) return;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        clear_selection();
        return;
    }

    if (key == GLFW_KEY_DELETE && action == GLFW_PRESS) {
        delete_selected();
        return;
    }

    handle_camera_keys(key, action);
}

void EditorController::handle_mouse_button(int button, int action, double x, double y) {
    float mx, my;
    // you may still want cursor_to_framebuffer here or move it in
    // for now assuming x/y are already in the space you need, or call the helper

    if (HUD::instance().mouse_button(mx, my, button, action)) return;

    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        rightMouseDown = (action == GLFW_PRESS);
        if (rightMouseDown) {
            lastMouseX = x;
            lastMouseY = y;
        }
        return;
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        handle_selection_click(x, y);
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        model.set_active_gizmo(GizmoAxis::NONE);
    }
}

void EditorController::handle_cursor(double x, double y) {
    float mx, my;
    // cursor_to_framebuffer(...) if needed
    HUD::instance().cursor(mx, my);

    if (rightMouseDown) {
        float dx = static_cast<float>(x - lastMouseX);
        float dy = static_cast<float>(y - lastMouseY);
        lastMouseX = x;
        lastMouseY = y;

        Camera& cam = model.get_active_camera();
        if (cam.get_mode() == CameraMode::ORTHO) {
            cam.add_azimuth(-dx * mouseLookSensitivity);
            cam.add_elevation(-dy * mouseLookSensitivity);
            // clamp elevation
            float el = cam.get_elevation();
            if (el > 179.0f) cam.set_elevation(179.0f);
            if (el < 1.0f)   cam.set_elevation(1.0f);
            update_ortho_eye();
        } else {
            cam.add_azimuth(dx * mouseLookSensitivity);
            cam.add_elevation(-dy * mouseLookSensitivity);
            float el = cam.get_elevation();
            if (el > 89.0f)  cam.set_elevation(89.0f);
            if (el < -89.0f) cam.set_elevation(-89.0f);
        }
        return;
    }

    // Gizmo dragging
    SceneNode* selected = model.get_selected();
    if (model.get_active_gizmo() == GizmoAxis::NONE || !selected) return;

    vmath::vec2 delta = vmath::vec2(mx, my) - dragStartMouse;

    vmath::vec3 axis(0.0f);
    if (model.get_active_gizmo() == GizmoAxis::X) axis = vmath::vec3(1, 0, 0);
    else if (model.get_active_gizmo() == GizmoAxis::Y) axis = vmath::vec3(0, 1, 0);
    else if (model.get_active_gizmo() == GizmoAxis::Z) axis = vmath::vec3(0, 0, 1);

    const Camera& cam = model.get_active_camera();
    const vmath::mat4& view = cam.get_view_matrix();

    // Transform axis into view space
    vmath::vec3 viewAxis;
    viewAxis[0] = view[0][0]*axis[0] + view[1][0]*axis[1] + view[2][0]*axis[2];
    viewAxis[1] = view[0][1]*axis[0] + view[1][1]*axis[1] + view[2][1]*axis[2];
    viewAxis[2] = view[0][2]*axis[0] + view[1][2]*axis[1] + view[2][2]*axis[2];

    vmath::vec2 screenDir(viewAxis[0], viewAxis[1]);
    float len = vmath::length(screenDir);
    if (len < 0.001f) len = 1.0f;
    screenDir = screenDir * (1.0f / len);

    // Distance-based sensitivity
    const float dist = vmath::length(cam.get_eye() - selected->position);
    const float sensitivity = 0.0015f * dist;

    const float amount = vmath::dot(delta, screenDir) * sensitivity;

    selected->position = dragStartPos + axis * amount;
}

void EditorController::handle_scroll(double xOffset, double yOffset) {
    Camera& cam = model.get_active_camera();
    if (cam.get_mode() != CameraMode::ORTHO) return;

    float r = cam.get_radius();
    r -= static_cast<float>(yOffset) * 0.4f;   // tune sensitivity
    if (r < 0.5f) r = 0.5f;
    cam.set_radius(r);
    update_ortho_eye();
}

void EditorController::handle_char(unsigned int codepoint) {
    HUD::instance().character(codepoint);
}

void EditorController::handle_resize(int width, int height) {
    glViewport(0, 0, width, height);
    HUD::instance().set_screen_size(width, height);
}

void EditorController::select_node(SceneNode* node) {
    model.set_selected(node);
}

void EditorController::clear_selection() {
    model.clear_selection();
}

void EditorController::delete_selected() {
    model.remove_selected();   // existing UI/scene function for now
    model.clear_selection();
}

void EditorController::toggle_camera_mode() {
    model.toggle_camera_mode();
}


void EditorController::handle_camera_keys(int key, int action) {
    if (model.get_active_camera().get_mode() == CameraMode::ORTHO)
        handle_ortho_keys(key, action);
    else
        handle_perspective_keys(key, action);
}

void EditorController::handle_selection_click(double x, double y) {
    HitResult hit = view.pick(model, static_cast<float>(x), static_cast<float>(y));

    if (hit.gizmo != GizmoAxis::NONE) {
        model.set_active_gizmo(hit.gizmo);
        dragStartPos = model.get_selected()->position;
        dragStartMouse = vmath::vec2(static_cast<float>(x), static_cast<float>(y));
        return;
    }

    select_node(hit.node);
}

void EditorController::update_ortho_eye() {
    Camera& cam = model.get_active_camera();
    if (cam.get_mode() != CameraMode::ORTHO) return;

    float az = cam.get_azimuth();
    float el = cam.get_elevation();
    float r  = cam.get_radius();

    float x = r * sin(vmath::deg2rad(az)) * sin(vmath::deg2rad(el));
    float y = r * cos(vmath::deg2rad(el));
    float z = r * cos(vmath::deg2rad(az)) * sin(vmath::deg2rad(el));
    cam.set_eye(vmath::vec3(x, y, z));
}