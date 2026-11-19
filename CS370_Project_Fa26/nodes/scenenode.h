//
// Created by ethan on 11/10/2026.
//

#ifndef CS370_FA26_SCENENODES_H
#define CS370_FA26_SCENENODES_H

struct SceneNode {
    string name;
    vec3 position{0.0f, 0.0f, 0.0f};
    vec3 rotation{0.0f, 0.0f, 0.0f};
    vec3 scale{1.0f, 1.0f, 1.0f};

    bool enabled = true;
    SceneNode* parent = nullptr;
    vector<SceneNode*> children;

    virtual ~SceneNode() {
        // detach from parent
        if (parent) {
            parent->remove_child(this);
        }
        // detach children (do not delete them — ownership stays with whoever created them)
        for (SceneNode* c : children) {
            if (c) c->parent = nullptr;
        }
        children.clear();
    }

    mat4 get_local_matrix() const {
        const mat4 T  = translate(position);
        const mat4 Rx = vmath::rotate(rotation[0], vec3(1.0f, 0.0f, 0.0f));
        const mat4 Ry = vmath::rotate(rotation[1], vec3(0.0f, 1.0f, 0.0f));
        const mat4 Rz = vmath::rotate(rotation[2], vec3(0.0f, 0.0f, 1.0f));
        const mat4 S  = vmath::scale(scale);
        return T * Rx * Ry * Rz * S;
    }

    mat4 get_world_matrix() const {
        mat4 local = get_local_matrix();
        if (parent) {
            return parent->get_world_matrix() * local;
        }
        return local;
    }

    void add_child(SceneNode* child) {
        if (!child || child == this) return;

        // already our child?
        for (const SceneNode* c : children) {
            if (c == child) return;
        }

        // remove from previous parent
        if (child->parent) {
            child->parent->remove_child(child);
        }

        child->parent = this;
        children.push_back(child);
    }

    void remove_child(SceneNode* child) {
        if (!child) return;

        for (auto it = children.begin(); it != children.end(); ++it) {
            if (*it == child) {
                children.erase(it);
                child->parent = nullptr;
                return;
            }
        }
    }



    virtual void write(std::ostream& out) const {
        out << "name " << name << "\n";
        out << "position " << position[0] << " " << position[1] << " " << position[2] << "\n";
        out << "rotation " << rotation[0] << " " << rotation[1] << " " << rotation[2] << "\n";
        out << "scale "    << scale[0]    << " " << scale[1]    << " " << scale[2]    << "\n";
        out << "enabled "  << (enabled ? 1 : 0) << "\n";
        out << "type " << get_type_name() << "\n";
    }

    // called for every key/value while loading
    virtual bool read_field(const std::string& key, std::istream& iss) {
        if (key == "name") {
            std::getline(iss, name);
            const size_t a = name.find_first_not_of(" \t\r\n");
            const size_t b = name.find_last_not_of(" \t\r\n");
            if (a == std::string::npos) name.clear();
            else name = name.substr(a, b - a + 1);
            return true;
        }
        if (key == "position") { iss >> position[0] >> position[1] >> position[2]; return true; }
        if (key == "rotation") { iss >> rotation[0] >> rotation[1] >> rotation[2]; return true; }
        if (key == "scale")    { iss >> scale[0] >> scale[1] >> scale[2]; return true; }
        if (key == "enabled")  { int e; iss >> e; enabled = e != 0; return true; }
        return false;   // unknown key
    }

    virtual const char* get_type_name() const { return "SceneNode"; }

    virtual void on_loaded() {}
    virtual void draw() = 0;
    virtual void on_select() {}
    virtual void update(float dt) {}
};











#endif //CS370_FA26_SCENENODES_H
