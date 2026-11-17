
#ifndef CS370_FA26_LABEL_H
#define CS370_FA26_LABEL_H

#include <utility>

#include "widget.h"


//Label for labeling UI sections
struct Label : Widget {
    string label;
    float textSize = HUD_FONT;
    vec4 color = HUD_LABEL;

    Label(const float x, const float y, const float w, const float h, string  label)
    : Widget(x, y, w, h), label(std::move(label)) {
        fit();
    }

    void set_text(const string &n_text) {
        label = n_text;
        fit();
    }

    void fit() {
        w = text_width(label, textSize) + HUD_PAD_X * 2.0f;
        h = textSize + HUD_PAD_Y * 2.0f + 4.0f;
        prefW = w;   // what the text actually needs
        prefH = h;
    }

    void draw_self() override {
        modelMatrix = mat4::identity();
        mat4 trans_matrix = mat4::identity();

        const mat4 scale_matrix = scale(w, h, 1.0f);
        trans_matrix = translate(x, static_cast<float>(hh) - h - y, 0.0f);
        modelMatrix = trans_matrix * scale_matrix;
        draw_solid_hud_quad(color);

        draw_string(label, x + 8.0f, y + 6.0f, textSize);
    }

};//end Label




#endif //CS370_FA26_LABEL_H
