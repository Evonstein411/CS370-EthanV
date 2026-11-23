#include "rendercontext.h"
#include "hud.h"                    // TEXT_COLOR, etc.
#include "../rendering/shaders.h"   // colorProgram, hudProgram, locations…
#include "../rendering/textures.h"  // fontTextureID
#include "../rendering/geometry.h"  // HUD_VAOs, HUD_ObjBuffers, glyphs, etc.
#include "../rendering/drawobjects.h"
#include <vector>
#include <../common/GLFWutils.h>                // or whatever GL header you use

// ------------------------------------------------------------------
// Orthographic projection for the HUD pass
// Origin at top-left, y increases downward (matches current widget code)
// ------------------------------------------------------------------
vmath::mat4 make_hud_ortho(int width, int height) {
    return vmath::ortho(0.0f, static_cast<float>(width),
                        static_cast<float>(height), 0.0f,
                        -1.0f, 1.0f);
}

// ------------------------------------------------------------------
// Solid colored quad (used by buttons, panels, etc.)
// ------------------------------------------------------------------
void RenderContext::draw_solid_quad(const vmath::vec4& color,
                                    const vmath::mat4& model) const {
    // reuse the existing dynamic color buffer setup from the old text.h
    if (dynamicHUDColorBuffer == 0) {
        glGenBuffers(1, &dynamicHUDColorBuffer);
        std::vector<vmath::vec4> tmp(HUD_numVertices[HUDQuad], color);
        glBindBuffer(GL_ARRAY_BUFFER, dynamicHUDColorBuffer);
        glBufferData(GL_ARRAY_BUFFER,
                     tmp.size() * sizeof(vmath::vec4),
                     tmp.data(), GL_DYNAMIC_DRAW);
    }

    std::vector<vmath::vec4> cols(HUD_numVertices[HUDQuad], color);
    glBindBuffer(GL_ARRAY_BUFFER, dynamicHUDColorBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    cols.size() * sizeof(vmath::vec4), cols.data());

    glUseProgram(colorProgram);
    glUniformMatrix4fv(colorProjMatLoc, 1, GL_FALSE, projMatrix);
    glUniformMatrix4fv(colorCamMatLoc,  1, GL_FALSE, cameraMatrix);
    glUniformMatrix4fv(colorModelMatLoc,1, GL_FALSE, model);

    glBindVertexArray(HUD_VAOs[HUDQuad]);
    glBindBuffer(GL_ARRAY_BUFFER, HUD_ObjBuffers[HUDQuad][PosBuffer]);
    glVertexAttribPointer(colorVPos, posCoords, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(colorVPos);

    glBindBuffer(GL_ARRAY_BUFFER, dynamicHUDColorBuffer);
    glVertexAttribPointer(colorVCol, colCoords, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(colorVCol);

    glDrawArrays(GL_TRIANGLES, 0, HUD_numVertices[HUDQuad]);
}

// ------------------------------------------------------------------
// Text
// ------------------------------------------------------------------
void RenderContext::draw_string(const std::string& text,
                                float x, float y, float size,
                                const vmath::vec4& color) const {
    const float scale = size / 32.0f;

    glUseProgram(hudProgram);
    glUniformMatrix4fv(hudProjMatLoc,   1, GL_FALSE, projMatrix);
    glUniformMatrix4fv(hudCameraMatLoc, 1, GL_FALSE, cameraMatrix);
    glUniform2f(hudUvScaleLoc,  1.0f, 1.0f);
    glUniform2f(hudUvOffsetLoc, 0.0f, 0.0f);

    if (GLint colorLoc = glGetUniformLocation(hudProgram, "textColor");
        colorLoc != -1)
        glUniform4fv(colorLoc, 1, color);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fontTextureID);

    float penX     = x;
    float baseline = y + size;

    for (char c : text) {
        if (c < 32 || c > 126) continue;

        const float startX    = penX;
        const float startBase = baseline;

        stbtt_aligned_quad q;
        stbtt_GetBakedQuad(glyphs, 512, 512, c - 32,
                           &penX, &baseline, &q, 1);

        const float gx0 = startX    + (q.x0 - startX)    * scale;
        const float gx1 = startX    + (q.x1 - startX)    * scale;
        const float gy0 = startBase + (q.y0 - startBase) * scale;
        const float gy1 = startBase + (q.y1 - startBase) * scale;

        penX     = startX + (penX - startX) * scale;
        baseline = startBase;

        const float top = screen_height() - gy0;
        const float bot = screen_height() - gy1;

        const vmath::vec4 pos[6] = {
            {gx0, top, 0, 1}, {gx1, top, 0, 1}, {gx1, bot, 0, 1},
            {gx0, top, 0, 1}, {gx1, bot, 0, 1}, {gx0, bot, 0, 1}
        };
        const vmath::vec2 uv[6] = {
            {q.s0, q.t0}, {q.s1, q.t0}, {q.s1, q.t1},
            {q.s0, q.t0}, {q.s1, q.t1}, {q.s0, q.t1}
        };

        glBindBuffer(GL_ARRAY_BUFFER, HUD_ObjBuffers[HUDTextQuad][PosBuffer]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), pos);
        glBindBuffer(GL_ARRAY_BUFFER, HUD_ObjBuffers[HUDTextQuad][TexBuffer]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(uv), uv);

        vmath::mat4 model = vmath::mat4::identity();
        glUniformMatrix4fv(hudModelMatLoc, 1, GL_FALSE, model);

        glBindVertexArray(HUD_VAOs[HUDTextQuad]);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}

float RenderContext::text_width(const std::string& text, float size) const {
    const float scale = size / 32.0f;
    float width = 0.0f;
    for (char c : text) {
        if (c < 32 || c > 126) continue;
        width += glyphs[c - 32].xadvance * scale;
    }
    return width;
}