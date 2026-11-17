#ifndef CS370_FA26_TEXT_H
#define CS370_FA26_TEXT_H

#include "widget.h"
#include "rendering/shaders.h"
#include "rendering/textures.h"
#include "rendering/geometry.h"
#include "rendering/drawobjects.h"
#include <string>
#include "../globals.h"


void draw_string(const string& text, float x, float y, float size,
                 const vec4 &color = TEXT_COLOR);
float text_width(const string& text, float size);

//util function for drawing text
inline void draw_string(const string& text, const float x, const float y, const float size, const vec4 &color) {
    const float scale = size / 32.0f;   // bake height is 32

    glUseProgram(hudProgram);
    glUniformMatrix4fv(hudProjMatLoc, 1, GL_FALSE, projMatrix);
    glUniformMatrix4fv(hudCameraMatLoc, 1, GL_FALSE, cameraMatrix);
    glUniform2f(hudUvScaleLoc, 1.0f, 1.0f);
    glUniform2f(hudUvOffsetLoc, 0.0f, 0.0f);

    if (GLint colorLoc = glGetUniformLocation(hudProgram, "textColor"); colorLoc != -1)
        glUniform4fv(colorLoc, 1, color);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fontTextureID);

    float penX = x;
    float baseline = y + size;    // y is pixels down from the top

    for (const char i : text) {
        if (i < 32 || i > 126) continue;

        const float startX = penX;
        const float startBase = baseline;
        stbtt_aligned_quad q;
        stbtt_GetBakedQuad(glyphs, 512, 512, i - 32, &penX, &baseline, &q, 1);

        const float gx0 = startX + (q.x0 - startX) * scale;
        const float gx1 = startX + (q.x1 - startX) * scale;
        const float gy0 = startBase + (q.y0 - startBase) * scale;
        const float gy1 = startBase + (q.y1 - startBase) * scale;
        penX = startX + (penX - startX) * scale;
        baseline = startBase;

        const float top = static_cast<float>(hh) - gy0;
        const float bot = static_cast<float>(hh) - gy1;
        const vec4 pos[6] = {
            vec4(gx0, top, 0, 1), vec4(gx1, top, 0, 1), vec4(gx1, bot, 0, 1),
            vec4(gx0, top, 0, 1), vec4(gx1, bot, 0, 1), vec4(gx0, bot, 0, 1)
        };
        const vec2 uv[6] = {
            vec2(q.s0, q.t0), vec2(q.s1, q.t0), vec2(q.s1, q.t1),
            vec2(q.s0, q.t0), vec2(q.s1, q.t1), vec2(q.s0, q.t1)
        };

        glBindBuffer(GL_ARRAY_BUFFER, HUD_ObjBuffers[HUDTextQuad][PosBuffer]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), pos);
        glBindBuffer(GL_ARRAY_BUFFER, HUD_ObjBuffers[HUDTextQuad][TexBuffer]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(uv), uv);

        modelMatrix = mat4::identity();
        draw_text_quad();
    }
}

//util function for calculating text width
inline float text_width(const string& text, float size) {
    const float scale = size / 32.0f;
    float width = 0.0f;
    for (char i : text) {
        if (i < 32 || i > 126) continue;
        width += glyphs[i - 32].xadvance * scale;
    }
    return width;
}

#endif //CS370_FA26_TEXT_H
