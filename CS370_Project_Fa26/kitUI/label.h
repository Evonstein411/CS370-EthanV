
#ifndef CS370_FA26_LABEL_H
#define CS370_FA26_LABEL_H

struct Label;
//Label for labeling UI sections
struct Label : Widget {
    string label;
    float textSize = HUD_FONT;
    vec4 color = HUD_GRAY;

    Label(float x, float y, float w, float h, const string& label)
    : Widget(x, y, w, h), label(label) {
        fit();
    }

    void setText(string n_text) {
        label = n_text;
        fit();
    }

    void fit() {
        w = text_width(label, textSize) + HUD_PAD_X * 2.0f;
        h = textSize + HUD_PAD_Y * 2.0f + 4.0f;
        prefW = w;   // what the text actually needs
        prefH = h;
    }

    void drawSelf() override {
        model_matrix = mat4().identity();
        mat4 scale_matrix, trans_matrix = mat4().identity();

        scale_matrix = scale(w, h, 1.0f);
        trans_matrix = translate(x, (float)hh - h - y, 0.0f);
        model_matrix = trans_matrix * scale_matrix;
        draw_solid_hud_quad(color);

        draw_string(label, x + 8.0f, y + 6.0f, textSize);
    }

};//end Label




#endif //CS370_FA26_LABEL_H
