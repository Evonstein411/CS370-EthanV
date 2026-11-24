//
// Created by ethan on 11/23/2026.
//

#ifndef CS370_FA26_RENDERCONTEXT_H
#define CS370_FA26_RENDERCONTEXT_H

#include "../common/vmath.h"
#include "hud.h"
#include <string>


struct RenderContext {
    int screenWidth  = 0;
    int screenHeight = 0;

    vmath::mat4 projMatrix   = vmath::mat4::identity();
    vmath::mat4 cameraMatrix = vmath::mat4::identity();

    float screen_height() const { return static_cast<float>(screenHeight); }
    float screen_width()  const { return static_cast<float>(screenWidth);  }

    void draw_solid_quad(const vmath::vec4& color, const vmath::mat4& model) const;
    void draw_string(const std::string& text, float x, float y, float size,
                     const vmath::vec4& color = TEXT_COLOR) const;
    float text_width(const std::string& text, float size) const;
};

vmath::mat4 make_hud_ortho(int width, int height);

#endif //CS370_FA26_RENDERCONTEXT_H
