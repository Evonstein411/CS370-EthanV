#ifndef CS370_FA26_APPEARANCE_H
#define CS370_FA26_APPEARANCE_H

#include <string>
#include "../common/vmath.h"
#include "../common/GLFWutils.h"
#include "../rendering/materials.h"   // MaterialProperties

enum class DrawMode { None, SolidColor, Material, Texture };

struct Appearance {
    DrawMode mode = DrawMode::None;

    GLuint colorBuffer = 0;
    vmath::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};

    Material material;
    int materialIndex = -1;

    GLuint texture = 0;
    std::string textureName;   // for save/load

    void build(GLuint mesh);    // creates the resources from the raw data
    void destroy();
};

#endif