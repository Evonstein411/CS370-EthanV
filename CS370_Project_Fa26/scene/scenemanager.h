//
// Created by ethan on 11/16/2026.
//
#ifndef CS370_FA26_SCENEMANAGER_H
#define CS370_FA26_SCENEMANAGER_H

#include "scene.h"
#include "../nodes/scenenode.h"
#include <vector>
#include <string>

struct SceneManager {
    std::vector<Scene*> scenes;
    Scene* active = nullptr;

    ~SceneManager();

    Scene* create_scene(const std::string& name = "Untitled");
    void set_active(Scene* scene);
    void close_scene(const Scene* scene);

    Scene* get_active() const;

    SceneNode* get_selected() const;
    void set_selected(SceneNode* node) const;
    void clear_active() const;
};



#endif //CS370_FA26_SCENEMANAGER_H
