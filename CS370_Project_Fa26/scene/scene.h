#ifndef CS370_FA26_SCENE_H
#define CS370_FA26_SCENE_H

#include <string>
#include <vector>
#include <functional>

struct SceneNode;

struct Scene {
    std::string name = "Untitled";
    std::string filePath;
    std::vector<SceneNode*> roots;
    SceneNode* selected = nullptr;

    Scene() = default;
    ~Scene();

    void clear();

    void add_root(SceneNode* node);
    void remove_root(const SceneNode* node);

    void for_each(const std::function<void(SceneNode*)>& fn) const;

    SceneNode* find_by_name(const std::string& name) const;

    bool save(const std::string& filename = "");
    bool load(const std::string& path);

private:
    static void walk(SceneNode* node, const std::function<void(SceneNode*)>& fn);
    static void write_node(std::ostream& out, const SceneNode* node);
    static SceneNode* read_node(std::istream& in);
};

#endif // CS370_FA26_SCENE_H
