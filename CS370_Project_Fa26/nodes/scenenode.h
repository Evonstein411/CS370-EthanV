#ifndef CS370_FA26_SCENENODE_H
#define CS370_FA26_SCENENODE_H

#include <string>
#include <vector>
#include <memory>
#include <iosfwd>   // forward declare streams
#include "component.h"
#include "../common/vmath.h"

struct SceneNode {
    std::string name;
    vmath::vec3 position{0.0f, 0.0f, 0.0f};
    vmath::vec3 rotation{0.0f, 0.0f, 0.0f};
    vmath::vec3 scale{1.0f, 1.0f, 1.0f};

    bool enabled = true;
    SceneNode* parent = nullptr;
    std::vector<SceneNode*> children;
    std::unique_ptr<Component> component;

    virtual ~SceneNode();

    vmath::mat4 get_local_matrix() const;

    vmath::mat4 get_world_matrix() const;

    void add_child(SceneNode* child);
    void remove_child(SceneNode* child);

    virtual const char* get_type_name() const { return "SceneNode"; }

    virtual void on_select() {}
    virtual void update(float dt) {}

    template<typename T>
    T* get_component() {
        return dynamic_cast<T*>(component.get());
    }

    template<typename T>
    const T* get_component() const {
        return dynamic_cast<T*>(component.get());
    }

    virtual void write(std::ostream& out) const;
    virtual bool read_field(const std::string& key, std::istream& iss);
    virtual void on_loaded();

};

#endif // CS370_FA26_SCENENODE_H