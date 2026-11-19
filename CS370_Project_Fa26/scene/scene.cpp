//
// Created by ethan on 11/19/2026.

#include "scene.h"
#include "../nodes/scenenode.h"
#include "../nodes/nodes.h"

#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>
#include <cstdio>


namespace fs = std::filesystem;

namespace {
    std::string trim(const std::string& s) {
        const size_t a = s.find_first_not_of(" \t\r\n");
        const size_t b = s.find_last_not_of(" \t\r\n");
        if (a == std::string::npos) return {};
        return s.substr(a, b - a + 1);
    }
}

Scene::~Scene() {
    clear();
}

void Scene::draw() const {
    for_each([](SceneNode* n) {
        if (n && n->enabled)
            n->draw();
    });
}

void Scene::clear() {
    std::vector<SceneNode*> all;
    for_each([&](SceneNode* n) { all.push_back(n); });

    for (SceneNode* n : all) {
        n->parent = nullptr;
        n->children.clear();
    }

    roots.clear();
    selected = nullptr;

    for (SceneNode* n : all)
        delete n;
}

void Scene::add_root(SceneNode* node) {
    if (!node || node->parent) return;
    roots.push_back(node);
}

void Scene::remove_root(const SceneNode* node) {
    if (!node) return;
    const auto it = std::find(roots.begin(), roots.end(), node);
    if (it == roots.end()) return;

    roots.erase(it);
    if (selected == node) selected = nullptr;
    delete node;
}

void Scene::for_each(const std::function<void(SceneNode*)>& fn) const {
    for (SceneNode* root : roots)
        walk(root, fn);
}

SceneNode* Scene::find_by_name(const std::string& name) const {
    SceneNode* found = nullptr;
    for_each([&](SceneNode* n) {
        if (!found && n && n->name == name)
            found = n;
    });
    return found;
}

bool Scene::save(const std::string& filename) {
    const std::string path = filename.empty() ? filePath : filename;
    if (path.empty()) {
        fprintf(stderr, "Scene::save — no path\n");
        return false;
    }

    const fs::path scenesDir = "../assets/scenes";
    if (!fs::exists(scenesDir))
        fs::create_directories(scenesDir);

    fs::path fullPath = scenesDir / path;
    if (fullPath.extension() != ".df")
        fullPath += ".df";

    std::ofstream out(fullPath);
    if (!out) {
        fprintf(stderr, "Failed to open %s\n", fullPath.string().c_str());
        return false;
    }

    out << "DESERTFOX_SCENE 2\n";
    for (SceneNode* root : roots)
        write_node(out, root);

    filePath = fullPath.string();
    printf("Saved scene to %s\n", filePath.c_str());
    return true;
}

bool Scene::load(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        fprintf(stderr, "Failed to open %s\n", path.c_str());
        return false;
    }

    clear();

    std::string line;
    if (!std::getline(in, line)) return false;   // skip header

    while (std::getline(in, line)) {
        line = trim(line);
        if (line == "NODE") {
            if (SceneNode* n = read_node(in))
                add_root(n);
        }
    }

    filePath = path;
    printf("Loaded scene from %s\n", path.c_str());
    return true;
}

void Scene::walk(SceneNode* node, const std::function<void(SceneNode*)>& fn) {
    if (!node) return;
    fn(node);
    for (SceneNode* child : node->children)
        walk(child, fn);
}

void Scene::write_node(std::ostream& out, const SceneNode* node) {
    if (!node) return;
    out << "NODE\n";
    node->write(out);
    out << "children " << node->children.size() << "\n";
    for (SceneNode* c : node->children)
        write_node(out, c);
    out << "END\n";
}

SceneNode* Scene::read_node(std::istream& in) {
    std::string line;
    std::string typeName = "MeshNode";
    std::vector<std::pair<std::string, std::string>> fields;
    int childCount = 0;

    while (std::getline(in, line)) {
        line = trim(line);
        if (line.empty()) continue;
        if (line == "END") break;

        std::istringstream iss(line);
        std::string key;
        iss >> key;

        if (key == "type") {
            std::getline(iss, typeName);
            typeName = trim(typeName);
        }
        else if (key == "children") {
            iss >> childCount;
            break;
        }
        else if (key != "NODE") {
            fields.emplace_back(key, line);
        }
    }

    SceneNode* node = create_node(typeName);
    if (!node) {
        fprintf(stderr, "Unknown node type '%s'\n", typeName.c_str());
        return nullptr;
    }

    for (auto& [key, value] : fields) {
        std::istringstream iss(value);
        std::string dummy;
        iss >> dummy;
        node->read_field(key, iss);
    }
    node->on_loaded();

    for (int i = 0; i < childCount; ++i) {
        std::string childLine;
        while (std::getline(in, childLine)) {
            childLine = trim(childLine);
            if (childLine == "NODE") {
                if (SceneNode* child = read_node(in))
                    node->add_child(child);
                break;
            }
            if (childLine == "END") break;
        }
    }

    // consume the closing END
    while (std::getline(in, line)) {
        if (trim(line) == "END") break;
    }

    return node;
}