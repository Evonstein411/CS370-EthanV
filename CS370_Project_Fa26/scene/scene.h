#ifndef CS370_FA26_SCENE_H
#define CS370_FA26_SCENE_H


#include <fstream>
#include <sstream>
#include <filesystem>
#include <functional>
#include <algorithm>


namespace fs = std::filesystem;

struct Scene {
    string name = "Untitled";
    string filePath;
    vector<SceneNode*> roots;// top-level nodes only
    SceneNode* selected = nullptr;

    ~Scene() { clear(); }

    void draw() {
        for_each([](SceneNode* n) {
            if (n->enabled)
                n->draw();
        });
    }

    void clear() {
        // collect every node first
        vector<SceneNode*> all;
        for_each([&](SceneNode* n) { all.push_back(n); });

        // break all links so destructors don't touch each other
        for (SceneNode* n : all) {
            n->parent = nullptr;
            n->children.clear();
        }

        roots.clear();
        selected = nullptr;

        // now safe to delete
        for (SceneNode* n : all)
            delete n;
    }

    void add_root(SceneNode* node) {
        if (!node || node->parent) return;
        roots.push_back(node);
    }

    void remove_root(SceneNode* node) {
        if (!node) return;
        auto it = std::find(roots.begin(), roots.end(), node);
        if (it != roots.end()) {
            roots.erase(it);
            if (selected == node) selected = nullptr;
            delete node;
        }
    }

    void for_each(const std::function<void(SceneNode*)>& fn) const {
        for (SceneNode* root : roots)
            walk(root, fn);
    }

    bool save(const string& filename = "") {
        // TODO: hierarchy serialization
        fprintf(stderr, "Scene::save not implemented yet\n");
        return false;
    }

    bool load(const string& path) {
        // TODO: hierarchy deserialization
        fprintf(stderr, "Scene::load not implemented yet\n");
        return false;
    }
    
    SceneNode* find_by_name(const string& name) const;

    static void walk(SceneNode* node, const std::function<void(SceneNode*)>& fn) {
        if (!node) return;
        fn(node);
        for (SceneNode* child : node->children)
            walk(child, fn);
    }


};



#endif //CS370_FA26_SCENE_H
