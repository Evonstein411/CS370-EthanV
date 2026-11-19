#ifndef CS370_FA26_APPEARANCE_H
#define CS370_FA26_APPEARANCE_H

#include "../common/vmath.h"
#include "../common/GLFWutils.h"
#include "../rendering/materials.h"   // MaterialProperties

enum class DrawMode { None, SolidColor, Material, Texture };

struct Appearance {
    DrawMode mode = DrawMode::None;
    vmath::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};
    Material material;
    GLuint texture = 0;
    std::string textureName;   // for save/load
};

#endif