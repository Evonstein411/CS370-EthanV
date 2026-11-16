//
// Created by ethan on 11/16/2026.
//

#ifndef CS370_FA26_DRAWTYPE_H
#define CS370_FA26_DRAWTYPE_H

#include <string>
#include <iostream>
#include "meshnode.h"


extern struct MeshNode;   // full definition comes from meshnode.h

struct DrawType {
    virtual ~DrawType() = default;
    virtual void initialize(MeshNode* mesh) = 0;
    virtual void draw(MeshNode* mesh) = 0;
    virtual const char* get_name() const = 0;
    virtual void write(std::ostream& out) const = 0;
    virtual bool read_field(const std::string& key, std::istream& iss) = 0;
};

// -------------------------------------------------------
struct SolidColorDraw : DrawType {
    vec4 color{1.0f, 1.0f, 1.0f, 1.0f};
    GLuint colorBuffer = 0;
    bool hasColor = false;

    const char* get_name() const override { return "Solid Color"; }

    void initialize(MeshNode* meshNode) override;
    void draw(MeshNode* meshNode) override;

    void write(std::ostream& out) const override {
        out << "color " << color[0] << " " << color[1] << " "
                        << color[2] << " " << color[3] << "\n";
    }

    bool read_field(const std::string& key, std::istream& iss) override {
        if (key == "color") {
            iss >> color[0] >> color[1] >> color[2] >> color[3];
            hasColor = true;
            return true;
        }
        return false;
    }
};

// -------------------------------------------------------
struct MaterialDraw : DrawType {
    MaterialProperties materialProperties;
    GLuint material = 0;
    bool hasMaterial = false;

    const char* get_name() const override { return "Material"; }

    void initialize(MeshNode* meshNode) override;
    void draw(MeshNode* meshNode) override;

    void write(std::ostream& out) const override {
        const auto& m = materialProperties;
        out << "ambient "   << m.ambient[0]  << " " << m.ambient[1]  << " " << m.ambient[2]  << " " << m.ambient[3]  << "\n";
        out << "diffuse "   << m.diffuse[0]  << " " << m.diffuse[1]  << " " << m.diffuse[2]  << " " << m.diffuse[3]  << "\n";
        out << "specular "  << m.specular[0] << " " << m.specular[1] << " " << m.specular[2] << " " << m.specular[3] << "\n";
        out << "shininess " << m.shininess << "\n";
    }

    bool read_field(const std::string& key, std::istream& iss) override {
        if (key == "ambient") {
            iss >> materialProperties.ambient[0] >> materialProperties.ambient[1]
                >> materialProperties.ambient[2] >> materialProperties.ambient[3];
            return true;
        }
        if (key == "diffuse") {
            iss >> materialProperties.diffuse[0] >> materialProperties.diffuse[1]
                >> materialProperties.diffuse[2] >> materialProperties.diffuse[3];
            return true;
        }
        if (key == "specular") {
            iss >> materialProperties.specular[0] >> materialProperties.specular[1]
                >> materialProperties.specular[2] >> materialProperties.specular[3];
            return true;
        }
        if (key == "shininess") {
            iss >> materialProperties.shininess;
            return true;
        }
        return false;
    }
};

// -------------------------------------------------------
struct TextureDraw : DrawType {
    GLuint texture = 0;

    const char* get_name() const override { return "Texture"; }

    void initialize(MeshNode* meshNode) override;
    void draw(MeshNode* meshNode) override;

    void write(std::ostream& out) const override {
        std::string texName = "blank";
        for (size_t i = 0; i < TextureIDs.size(); ++i)
            if (TextureIDs[i] == texture) { texName = textureNames[i]; break; }
        out << "texture " << texName << "\n";
    }

    bool read_field(const std::string& key, std::istream& iss) override {
        if (key == "texture") {
            std::string texName;
            iss >> texName;
            texture = TextureIDs[Blank];
            for (size_t i = 0; i < textureNames.size(); ++i)
                if (textureNames[i] == texName) { texture = TextureIDs[i]; break; }
            return true;
        }
        return false;
    }
};

// -------------------------------------------------------
// factory
inline DrawType* create_draw_type(const std::string& name) {
    if (name == "Solid Color") return new SolidColorDraw();
    if (name == "Material")    return new MaterialDraw();
    if (name == "Texture")     return new TextureDraw();
    return nullptr;
}







#endif //CS370_FA26_DRAWTYPE_H
