//
// Created by ethan on 11/16/2026.
//

#ifndef CS370_FA26_MESHNODE_H
#define CS370_FA26_MESHNODE_H

#include "scenenode.h"
#include "drawtype.h"

struct MeshNode : SceneNode {
    GLuint mesh = static_cast<GLuint>(-1);
    DrawType* drawType = nullptr;

    explicit MeshNode(string nodeName, const GLuint meshId = static_cast<GLuint>(-1))
            : mesh(meshId)
    {
        name = std::move(nodeName);
    }

    ~MeshNode() override {
        delete drawType;
        drawType = nullptr;
    }

    void set_draw_type(DrawType* newType) {
        delete drawType;
        drawType = newType;
        if (drawType)
            drawType->initialize(this);
    }

    void draw() override {
        if (!enabled || mesh == static_cast<GLuint>(-1) || !drawType) return;
        modelMatrix = get_world_matrix();
        drawType->draw(this);
    }

    // ---- convenience creators (match old SceneObject constructors) ----
    static MeshNode* create_solid_color(string name, const GLuint mesh,
                                        const vec3 &pos, const vec3 &scale, const vec3 &rot, const vec4 &color)
    {
        auto* node = new MeshNode(std::move(name), mesh);
        node->position = pos;
        node->scale = scale;
        node->rotation = rot;

        auto* d = new SolidColorDraw();
        d->color = color;
        node->set_draw_type(d);
        return node;
    }

    static MeshNode* create_material(string name, const GLuint mesh,
                                     const vec3 &pos, const vec3 &scale, const vec3 &rot,
                                     const MaterialProperties &mat)
    {
        auto* node = new MeshNode(std::move(name), mesh);
        node->position = pos;
        node->scale = scale;
        node->rotation = rot;

        auto* d = new MaterialDraw();
        d->materialProperties = mat;
        node->set_draw_type(d);
        return node;
    }

    static MeshNode* create_texture(string name, const GLuint mesh,
                                    const vec3 &pos, const vec3 &scale, const vec3 &rot, const GLuint texture)
    {
        auto* node = new MeshNode(std::move(name), mesh);
        node->position = pos;
        node->scale = scale;
        node->rotation = rot;

        auto* d = new TextureDraw();
        d->texture = texture;
        node->set_draw_type(d);
        return node;
    }

    const char* get_type_name() const override { return "MeshNode"; }

    void write(std::ostream& out) const override {
        SceneNode::write(out);                 // common fields first

        std::string meshName = "none";
        if (mesh != static_cast<GLuint>(-1) && mesh < meshNames.size())
            meshName = meshNames[mesh];
        out << "mesh " << meshName << "\n";

        if (drawType) {
            out << "drawType " << drawType->get_name() << "\n";
            drawType->write(out);              // strategy writes itself
        } else {
            out << "drawType None\n";
        }
    }

    bool read_field(const std::string& key, std::istream& iss) override {
        if (SceneNode::read_field(key, iss)) return true;

        if (key == "mesh") {
            std::string meshName;
            iss >> meshName;
            if (const int idx = find_mesh(meshName); idx >= 0) mesh = static_cast<GLuint>(idx);
            return true;
        }
        if (key == "drawType") {
            std::string typeName;
            std::getline(iss, typeName);
            const size_t a = typeName.find_first_not_of(" \t\r\n");
            const size_t b = typeName.find_last_not_of(" \t\r\n");
            if (a != std::string::npos)
                typeName = typeName.substr(a, b - a + 1);
            else
                typeName.clear();

            delete drawType;
            drawType = create_draw_type(typeName);   // do NOT call initialize yet
            return true;
        }
        // let the current DrawType try
        if (drawType && drawType->read_field(key, iss)) return true;
        return false;
    }

    void on_loaded() override {
        if (drawType)
            drawType->initialize(this);
    }


};//end MeshNode

//drawType functions that need mesh node to function
inline void SolidColorDraw::initialize(MeshNode* meshNode) {
    if (!meshNode || meshNode->mesh == static_cast<GLuint>(-1)) return;
    if (!hasColor) {
        color = {1.0f, 1.0f, 1.0f, 1.0f};
        hasColor = true;
    }
    GLuint buf;
    glGenBuffers(1, &buf);
    ColorBuffers.push_back(buf);
    colorBuffer = static_cast<GLuint>(ColorBuffers.size()) - 1;
    build_solid_color_buffer(numVertices[meshNode->mesh], color, colorBuffer);
}

inline void SolidColorDraw::draw(MeshNode* meshNode) {
    draw_color_object(meshNode->mesh, colorBuffer);
}

inline void MaterialDraw::initialize(MeshNode* /*meshNode*/) {
    if (!hasMaterial) {
        materialProperties = baseMaterial;
        hasMaterial = true;
    }
    material = static_cast<GLuint>(dynamicMaterials.size());
    dynamicMaterials.push_back(materialProperties);
}

inline void MaterialDraw::draw(MeshNode* meshNode) {
    normalMatrix = modelMatrix.inverse().transpose();
    const GLuint shaderIndex = static_cast<GLuint>(materials.size()) + material;
    draw_mat_object(meshNode->mesh, shaderIndex);
}

inline void TextureDraw::initialize(MeshNode* /*meshNode*/) {
    if (!texture)
        texture = TextureIDs[Blank];
}

inline void TextureDraw::draw(MeshNode* meshNode) {
    draw_tex_object(meshNode->mesh, texture);
}



#endif //CS370_FA26_MESHNODE_H
