//
// Created by ethan on 11/16/2026.
//

#ifndef CS370_FA26_MESHNODE_H
#define CS370_FA26_MESHNODE_H

#include "scenenode.h"

struct MeshNode;


// -------------------------------------------------------
// DrawType hierarchy
// -------------------------------------------------------

struct DrawType {
    virtual ~DrawType() = default;
    virtual void initialize(MeshNode* mesh) = 0;
    virtual void draw(MeshNode* mesh) = 0;
    virtual const char* get_name() const = 0;
    // future: virtual void build_ui(Panel* panel) {}
};

struct SolidColorDraw : DrawType {
    vec4 color{1.0f};
    GLuint colorBuffer = 0;
    bool hasColor = false;

    void initialize(MeshNode* mesh) override;
    void draw(MeshNode* mesh) override;
    const char* get_name() const override { return "Solid Color"; }
};

struct MaterialDraw : DrawType {
    MaterialProperties materialProperties;
    GLuint material = 0;          // index into dynamicMaterials
    bool hasMaterial = false;

    void initialize(MeshNode* mesh) override;
    void draw(MeshNode* mesh) override;
    const char* get_name() const override { return "Material"; }
};

struct TextureDraw : DrawType {
    GLuint texture = 0;

    void initialize(MeshNode* mesh) override;
    void draw(MeshNode* mesh) override;
    const char* get_name() const override { return "Texture"; }
};


// -------------------------------------------------------
// MeshNode
// -------------------------------------------------------

struct MeshNode : SceneNode {
    GLuint mesh = (GLuint)-1;
    DrawType* drawType = nullptr;

    MeshNode(string nodeName, GLuint meshId = (GLuint)-1)
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
        if (!enabled || mesh == (GLuint)-1 || !drawType) return;
        modelMatrix = get_world_matrix();
        drawType->draw(this);
    }

    // ---- convenience creators (match old SceneObject constructors) ----
    static MeshNode* create_solid_color(string name, GLuint mesh,
                                        vec3 pos, vec3 scale, vec3 rot, vec4 color)
    {
        MeshNode* node = new MeshNode(std::move(name), mesh);
        node->position = pos;
        node->scale = scale;
        node->rotation = rot;

        auto* d = new SolidColorDraw();
        d->color = color;
        node->set_draw_type(d);
        return node;
    }

    static MeshNode* create_material(string name, GLuint mesh,
                                     vec3 pos, vec3 scale, vec3 rot,
                                     MaterialProperties mat)
    {
        MeshNode* node = new MeshNode(std::move(name), mesh);
        node->position = pos;
        node->scale = scale;
        node->rotation = rot;

        auto* d = new MaterialDraw();
        d->materialProperties = mat;
        node->set_draw_type(d);
        return node;
    }

    static MeshNode* create_texture(string name, GLuint mesh,
                                    vec3 pos, vec3 scale, vec3 rot, GLuint texture)
    {
        MeshNode* node = new MeshNode(std::move(name), mesh);
        node->position = pos;
        node->scale = scale;
        node->rotation = rot;

        auto* d = new TextureDraw();
        d->texture = texture;
        node->set_draw_type(d);
        return node;
    }


};

// -------------------------------------------------------
// DrawType implementations
// -------------------------------------------------------
inline void SolidColorDraw::initialize(MeshNode* meshNode) {
    if (!hasColor) {
        color = {1.0f, 1.0f, 1.0f, 1.0f};
        hasColor = true;
    }
    GLuint buf;
    glGenBuffers(1, &buf);
    ColorBuffers.push_back(buf);
    colorBuffer = (GLuint)ColorBuffers.size() - 1;
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
    material = (GLuint)dynamicMaterials.size();
    dynamicMaterials.push_back(materialProperties);
}

inline void MaterialDraw::draw(MeshNode* meshNode) {
    normalMatrix = modelMatrix.inverse().transpose();
    GLuint shaderIndex = (GLuint)materials.size() + material;
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
