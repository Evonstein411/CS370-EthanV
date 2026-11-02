#ifndef CS370_FA26_TEXT_H
#define CS370_FA26_TEXT_H

void draw_string(const string& text, float x, float y, float size,
                 vec4 color = TEXT_COLOR);
float text_width(const string& text, float size);

//util function for drawing text
void draw_string(const string& text, float x, float y, float size, vec4 color) {
    float scale = size / 32.0f;   // bake height is 32

    glUseProgram(hud_program);
    glUniformMatrix4fv(hud_proj_mat_loc, 1, GL_FALSE, proj_matrix);
    glUniformMatrix4fv(hud_camera_mat_loc, 1, GL_FALSE, camera_matrix);
    glUniform2f(hud_uv_scale_loc, 1.0f, 1.0f);
    glUniform2f(hud_uv_offset_loc, 0.0f, 0.0f);

    GLint colorLoc = glGetUniformLocation(hud_program, "textColor");
    if (colorLoc != -1)
        glUniform4fv(colorLoc, 1, color);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fontTextureID);

    float penX = x;
    float baseline = y + size;    // y is pixels down from the top

    for (size_t i = 0; i < text.size(); i++) {
        if (text[i] < 32 || text[i] > 126) continue;

        float startX = penX;
        float startBase = baseline;
        stbtt_aligned_quad q;
        stbtt_GetBakedQuad(glyphs, 512, 512, text[i] - 32, &penX, &baseline, &q, 1);

        float gx0 = startX + (q.x0 - startX) * scale;
        float gx1 = startX + (q.x1 - startX) * scale;
        float gy0 = startBase + (q.y0 - startBase) * scale;
        float gy1 = startBase + (q.y1 - startBase) * scale;
        penX = startX + (penX - startX) * scale;
        baseline = startBase;

        float top = (float)hh - gy0;
        float bot = (float)hh - gy1;
        vec4 pos[6] = {
            vec4(gx0, top, 0, 1), vec4(gx1, top, 0, 1), vec4(gx1, bot, 0, 1),
            vec4(gx0, top, 0, 1), vec4(gx1, bot, 0, 1), vec4(gx0, bot, 0, 1)
        };
        vec2 uv[6] = {
            vec2(q.s0, q.t0), vec2(q.s1, q.t0), vec2(q.s1, q.t1),
            vec2(q.s0, q.t0), vec2(q.s1, q.t1), vec2(q.s0, q.t1)
        };

        glBindBuffer(GL_ARRAY_BUFFER, HUD_ObjBuffers[HUDTextQuad][PosBuffer]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), pos);
        glBindBuffer(GL_ARRAY_BUFFER, HUD_ObjBuffers[HUDTextQuad][TexBuffer]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(uv), uv);

        model_matrix = mat4().identity();
        draw_text_quad();
    }
}

//util function for calculating text width
float text_width(const string& text, float size) {
    float scale = size / 32.0f;
    float width = 0.0f;
    for (size_t i = 0; i < text.size(); i++) {
        if (text[i] < 32 || text[i] > 126) continue;
        width += glyphs[text[i] - 32].xadvance * scale;
    }
    return width;
}


#endif //CS370_FA26_TEXT_H
