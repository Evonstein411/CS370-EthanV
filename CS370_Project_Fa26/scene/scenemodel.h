//
// Created by ethan on 11/18/2026.
//

#ifndef CS370_FA26_SCENEMODEL_H
#define CS370_FA26_SCENEMODEL_H

#include "scenemanager.h"
#include "../nodes/scenenode.h"
#include "../rendering/camera.h"
#include "../rendering/renderer.h"

class SceneModel {
public:
    SceneModel();
    ~SceneModel() = default;

    // Scene lifetime
    void create_default_scene();
    void clear();
    Scene* get_active_scene();
    const Scene* get_active_scene() const;

    // Selection
    SceneNode* get_selected() const;
    void set_selected(SceneNode* node);
    void clear_selection();
    void remove_selected();
    void remove_node(SceneNode* node);
    GizmoAxis get_active_gizmo() const { return activeGizmo; }
    void set_active_gizmo(GizmoAxis axis) { activeGizmo = axis; }

    // SceneManager
    SceneManager& get_scene_manager();
    const SceneManager& get_scene_manager() const;


    //Cameras
    Camera& get_active_camera();
    const Camera& get_active_camera() const;
    Camera& get_ortho_camera();
    Camera& get_perspective_camera();
    void set_active_camera(CameraMode mode);
    CameraMode get_active_camera_mode() const;
    void toggle_camera_mode();

private:
    SceneManager sceneManager;
    GizmoAxis activeGizmo = GizmoAxis::NONE;

    Camera orthoCamera{CameraMode::ORTHO};
    Camera perspectiveCamera{CameraMode::PERSPECTIVE};
    Camera* activeCamera = &orthoCamera;
};



#endif //CS370_FA26_SCENEMODEL_H
