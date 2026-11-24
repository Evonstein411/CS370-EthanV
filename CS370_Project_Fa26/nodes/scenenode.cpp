#include "scenenode.h"

#include <iostream>
#include <algorithm>

SceneNode::~SceneNode() {
    if (parent)
        parent->remove_child(this);

    for (SceneNode* c : children) {
        if (c) c->parent = nullptr;
    }
    children.clear();
}

void SceneNode::set_component(std::unique_ptr<Component> c) {
    component = std::move(c);
}

vmath::mat4 SceneNode::get_local_matrix() const {
    const vmath::mat4 T  = translate(position);
    const vmath::mat4 Rx = vmath::rotate(rotation[0], vmath::vec3(1.0f, 0.0f, 0.0f));
    const vmath::mat4 Ry = vmath::rotate(rotation[1], vmath::vec3(0.0f, 1.0f, 0.0f));
    const vmath::mat4 Rz = vmath::rotate(rotation[2], vmath::vec3(0.0f, 0.0f, 1.0f));
    const vmath::mat4 S  = vmath::scale(scale);
    return T * Rx * Ry * Rz * S;
}

vmath::mat4 SceneNode::get_world_matrix() const {
    const vmath::mat4 local = get_local_matrix();
    if (parent)
        return parent->get_world_matrix() * local;
    return local;
}

void SceneNode::add_child(SceneNode* child) {
    if (!child || child == this) return;

    for (const SceneNode* c : children) {
        if (c == child) return;
    }

    if (child->parent)
        child->parent->remove_child(child);

    child->parent = this;
    children.push_back(child);
}

void SceneNode::remove_child(SceneNode* child) {
    if (!child) return;

    for (auto it = children.begin(); it != children.end(); ++it) {
        if (*it == child) {
            children.erase(it);
            child->parent = nullptr;
            return;
        }
    }
}

void SceneNode::write(std::ostream& out) const {
    out << "name " << name << "\n";
    out << "position " << position[0] << " " << position[1] << " " << position[2] << "\n";
    out << "rotation " << rotation[0] << " " << rotation[1] << " " << rotation[2] << "\n";
    out << "scale "    << scale[0]    << " " << scale[1]    << " " << scale[2]    << "\n";
    out << "enabled "  << (enabled ? 1 : 0) << "\n";

    if (component) {
        out << "component " << component->get_type_name() << "\n";
        component->write(out);
    }
}

bool SceneNode::read_field(const std::string& key, std::istream& iss) {
    if (key == "name") { /* trim and assign */ return true; }
    if (key == "position") { iss >> position[0] >> position[1] >> position[2]; return true; }
    if (key == "rotation") { iss >> rotation[0] >> rotation[1] >> rotation[2]; return true; }
    if (key == "scale")    { iss >> scale[0] >> scale[1] >> scale[2]; return true; }
    if (key == "enabled")  { int e; iss >> e; enabled = e != 0; return true; }

    // forward unknown fields to the component if we have one
    if (component && component->read_field(key, iss))
        return true;

    return false;
}

void SceneNode::on_loaded() {
    if (component)
        component->on_loaded();
}