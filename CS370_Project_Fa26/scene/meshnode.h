//
// Created by ethan on 11/16/2026.
//

#ifndef CS370_FA26_MESHNODE_H
#define CS370_FA26_MESHNODE_H

#include "scenenode.h"

struct MeshNode;

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



struct MeshNode : SceneNode {
    GLuint mesh = (GLuint)-1;
    DrawType* drawType = nullptr;

    MeshNode(string name, GLuint mesh = (GLuint)-1);
    ~MeshNode() override;

    void set_draw_type(DrawType* newType); // replaces initialize_type()
    void draw() override;

    // convenience constructors that match the old SceneObject ones
    // (they just create the right DrawType and call set_draw_type)
    static MeshNode* create_solid_color(string name, GLuint mesh, vec3 pos, vec3 scale, vec3 rot, vec4 color);
    static MeshNode* create_material(string name, GLuint mesh, vec3 pos, vec3 scale, vec3 rot, MaterialProperties mat);
    static MeshNode* create_texture(string name, GLuint mesh, vec3 pos, vec3 scale, vec3 rot, GLuint texture);


    void MeshNode::draw() {
        if (!enabled || mesh == (GLuint)-1 || !drawType) return;
        modelMatrix = get_world_matrix();   // note: world, not local
        drawType->draw(this);
    }

};





#endif //CS370_FA26_MESHNODE_H
