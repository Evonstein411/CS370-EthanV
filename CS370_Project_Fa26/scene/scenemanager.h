//
// Created by ethan on 11/16/2026.
//
#ifndef CS370_FA26_SCENEMANAGER_H
#define CS370_FA26_SCENEMANAGER_H

#include "scene.h"
#include "scenenode.h"

struct SceneManager {
    vector<Scene*> scenes;
    Scene* active = nullptr;

    ~SceneManager() {
        for (Scene* s : scenes)
            delete s;
        scenes.clear();
        active = nullptr;
    }

    Scene* create_scene(const string& name = "Untitled") {
        Scene* scene = new Scene();
        scene->name = name;
        scenes.push_back(scene);
        if (!active)
            active = scene;
        return scene;
    }

    void set_active(Scene* scene) {
        if (!scene) return;
        // only allow scenes we own
        if (std::find(scenes.begin(), scenes.end(), scene) == scenes.end())
            return;
        active = scene;
    }

    void close_scene(Scene* scene) {
        if (!scene) return;
        auto it = std::find(scenes.begin(), scenes.end(), scene);
        if (it == scenes.end()) return;

        scenes.erase(it);
        if (active == scene) {
            active = scenes.empty() ? nullptr : scenes.back();
        }
        delete scene;
    }

    Scene* get_active() const { return active; }

    // convenience — forwards to the active scene
    SceneNode* get_selected() const {
        return active ? active->selected : nullptr;
    }

    void set_selected(SceneNode* node) {
        if (active)
            active->selected = node;
    }

    void clear_active() {
        if (active)
            active->clear();
    }


};

inline SceneManager sceneManager; //single global sceneManager for now


#endif //CS370_FA26_SCENEMANAGER_H
