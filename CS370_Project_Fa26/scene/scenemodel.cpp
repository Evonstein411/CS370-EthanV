//
// Created by ethan on 11/18/2026.
//
#include "scenemodel.h"
#include "rendering/materials.h"
#include "rendering/geometry.h"
#include "../nodes/scenenode.h"
#include "nodes/meshcomponent.h"

SceneModel::SceneModel() {
    // optional: set distinct starting positions if you want
    orthoCamera.set_eye({3.0f, 3.0f, 3.0f});
    orthoCamera.set_radius(17.5f);

    perspectiveCamera.set_eye({0.0f, 2.0f, 10.0f});
    perspectiveCamera.set_azimuth(0.0f);
    perspectiveCamera.set_elevation(0.0f);
}


void SceneModel::create_default_scene() {
    sceneManager.create_scene("Desert");
    Scene* scene = sceneManager.get_active();
    if (!scene) return;

    const int cubeID = find_mesh("cube");
    const int sphereID = find_mesh("sphere");
    if (cubeID < 0 || sphereID < 0) {
        fprintf(stderr, "Required models missing from ../assets/models\n");
        return;
    }

    //lambda for building material meshes
    auto add_material_mesh = [&](const std::string& name, GLuint mesh,
                                 const vmath::vec3& pos, const vmath::vec3& scale, const vmath::vec3& rot,
                                 const Material& mat) {
        auto* node = new SceneNode();
        node->name = name;
        node->position = pos;
        node->scale = scale;
        node->rotation = rot;

        auto meshComp = std::make_unique<MeshComponent>();
        meshComp->mesh = mesh;
        meshComp->appearance.mode = DrawMode::Material;
        meshComp->appearance.material = mat;
        meshComp->on_loaded();                 // builds the material index / UBO entry

        node->component = std::move(meshComp);
        scene->add_root(node);
    };

    add_material_mesh("sphere", sphereID, {0,0,0}, {2,2,2}, {0,0,0}, baseMaterial);
    add_material_mesh("floor",  cubeID,   {0,-3,0}, {50,2,50}, {0,0,0}, baseMaterial);
    add_material_mesh("wall1",  cubeID,   {25,3,0}, {1,10,50}, {0,0,0}, baseMaterial);
    add_material_mesh("wall2",  cubeID,   {-25,3,0}, {1,10,50}, {0,0,0}, baseMaterial);
    add_material_mesh("wall3",  cubeID,   {0,3,25}, {50,10,1}, {0,0,0}, baseMaterial);
    add_material_mesh("wall4",  cubeID,   {0,3,-25}, {50,10,1}, {0,0,0}, baseMaterial);

}

void SceneModel::clear() {
    sceneManager.clear_active();
}

Scene* SceneModel::get_active_scene() {
    return sceneManager.get_active();
}

const Scene* SceneModel::get_active_scene() const {
    return sceneManager.get_active();
}

SceneNode* SceneModel::get_selected() const {
    return sceneManager.get_selected();
}

void SceneModel::set_selected(SceneNode* node) {
    sceneManager.set_selected(node);
}

void SceneModel::clear_selection() {
    sceneManager.set_selected(nullptr);
}

SceneManager& SceneModel::get_scene_manager() {
    return sceneManager;
}

const SceneManager& SceneModel::get_scene_manager() const {
    return sceneManager;
}

Camera& SceneModel::get_active_camera() {
    return *activeCamera;
}

const Camera& SceneModel::get_active_camera() const {
    return *activeCamera;
}

Camera& SceneModel::get_ortho_camera() {
    return orthoCamera;
}

Camera& SceneModel::get_perspective_camera() {
    return perspectiveCamera;
}

void SceneModel::set_active_camera(CameraMode mode) {
    activeCamera = (mode == CameraMode::PERSPECTIVE) ? &perspectiveCamera : &orthoCamera;
}

CameraMode SceneModel::get_active_camera_mode() const {
    return activeCamera->get_mode();
}

void SceneModel::toggle_camera_mode() {
    activeCamera = (activeCamera == &orthoCamera) ? &perspectiveCamera : &orthoCamera;
}