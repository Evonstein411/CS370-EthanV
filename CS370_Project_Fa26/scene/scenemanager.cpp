#include "scenemanager.h"

SceneManager::~SceneManager() {
    for (Scene* s : scenes)
        delete s;
    scenes.clear();
    active = nullptr;
}

Scene* SceneManager::create_scene(const std::string& name) {
    auto* scene = new Scene();
    scene->name = name;
    scenes.push_back(scene);
    if (!active)
        active = scene;
    return scene;
}

void SceneManager::set_active(Scene* scene) {
    if (!scene) return;
    if (std::find(scenes.begin(), scenes.end(), scene) == scenes.end())
        return;
    active = scene;
}

void SceneManager::close_scene(const Scene* scene) {
    if (!scene) return;
    const auto it = std::find(scenes.begin(), scenes.end(), scene);
    if (it == scenes.end()) return;

    scenes.erase(it);
    if (active == scene)
        active = scenes.empty() ? nullptr : scenes.back();
    delete scene;
}

Scene* SceneManager::get_active() const {
    return active;
}

SceneNode* SceneManager::get_selected() const {
    return active ? active->selected : nullptr;
}

void SceneManager::set_selected(SceneNode* node) const {
    if (active)
        active->selected = node;
}

void SceneManager::clear_active() const {
    if (active)
        active->clear();
}