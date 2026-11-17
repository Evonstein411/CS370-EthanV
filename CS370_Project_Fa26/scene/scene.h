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
        string path = filename.empty() ? filePath : filename;
        if (path.empty()) {
            fprintf(stderr, "Scene::save — no path\n");
            return false;
        }

        fs::path scenesDir = "../assets/scenes";
        if (!fs::exists(scenesDir))
            fs::create_directories(scenesDir);

        fs::path fullPath = scenesDir / path;
        if (fullPath.extension() != ".df")
            fullPath += ".df";

        ofstream out(fullPath);
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

    bool load(const string& path) {
        ifstream in(path);
        if (!in) {
            fprintf(stderr, "Failed to open %s\n", path.c_str());
            return false;
        }

        clear();

        string line;
        if (!getline(in, line)) return false;   // skip header

        auto trim = [](string s) {
            size_t a = s.find_first_not_of(" \t\r\n");
            size_t b = s.find_last_not_of(" \t\r\n");
            if (a == string::npos) return string{};
            return s.substr(a, b - a + 1);
        };

        while (getline(in, line)) {
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

    SceneNode* find_by_name(const string& name) const;

private:
    static void walk(SceneNode* node, const std::function<void(SceneNode*)>& fn) {
        if (!node) return;
        fn(node);
        for (SceneNode* child : node->children)
            walk(child, fn);
    }

    void write_node(ostream& out, SceneNode* node) {
        if (!node) return;
        out << "NODE\n";
        node->write(out);                          // virtual
        out << "children " << node->children.size() << "\n";
        for (SceneNode* c : node->children)
            write_node(out, c);
        out << "END\n";
    }

    SceneNode* read_node(istream& in) {
        string line;
        string typeName = "MeshNode";
        vector<pair<string, string>> fields;
        int childCount = 0;

        auto trim = [](string s) {
            size_t a = s.find_first_not_of(" \t\r\n");
            size_t b = s.find_last_not_of(" \t\r\n");
            if (a == string::npos) return string{};
            return s.substr(a, b - a + 1);
        };

        while (getline(in, line)) {
            line = trim(line);
            if (line.empty()) continue;
            if (line == "END") break;

            istringstream iss(line);
            string key;
            iss >> key;

            if (key == "type") {
                getline(iss, typeName);
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

        // feed fields — MeshNode::read_field must be null-safe
        for (auto& f : fields) {
            istringstream iss(f.second);
            string dummy;
            iss >> dummy;
            node->read_field(f.first, iss);
        }
        node->on_loaded();

        // children
        for (int i = 0; i < childCount; ++i) {
            string childLine;
            while (getline(in, childLine)) {
                childLine = trim(childLine);
                if (childLine == "NODE") {
                    if (SceneNode* child = read_node(in))
                        node->add_child(child);
                    break;
                }
                if (childLine == "END") break;
            }
        }

        // consume the closing END (for childCount == 0 this eats the END right after "children 0")
        while (getline(in, line)) {
            if (trim(line) == "END") break;
        }

        return node;
    }
};



#endif //CS370_FA26_SCENE_H
