//
// Created by ethan on 11/2/2026.
//

#ifndef CS370_FA26_BUTTON_H
#define CS370_FA26_BUTTON_H


struct Button;

//Button widget for interactivity
struct Button : Widget {
    string label;
    float textSize = HUD_FONT;
    std::function<void()> onPress;
    vec4 color = HUD_BUTTON;
    vec4 highlightedColor = HUD_HIGHLIGHTED;

    //constructor (no label specified)
    Button(float x, float y, float w, float h, std::function<void()> onPress, const string& label = "" )
    : Widget(x, y, w, h), onPress(onPress), label(label) {
        fit();
        if (prefW <= 0.0f) prefW = w;   // explicit size, no label
        if (prefH <= 0.0f) prefH = h;
    }

    void fit() {
        if (label != "") {
            w = text_width(label, textSize) + HUD_PAD_X * 2.0f;
            if (w < 48.0f) w = 48.0f;
            h = textSize + HUD_PAD_Y * 2.0f + 4.0f;
            prefW = w;
            prefH = h;
        }
    }

    void drawSelf() override {
        model_matrix = mat4().identity();
        mat4 scale_matrix, trans_matrix = mat4().identity();


        scale_matrix = scale(w, h, 1.0f);
        trans_matrix = translate(x, (float)hh - h - y, 0.0f);//move to top left for more intuitive positioning increase in y goes down
        model_matrix = trans_matrix * scale_matrix;
        draw_solid_hud_quad(hovered ? highlightedColor: color);

        draw_string(label, x + HUD_PAD_X, y + HUD_PAD_Y, textSize);
    }

    void on_click(float mx, float my) override {
        if (onPress) onPress();
    }

};//end Button

#endif //CS370_FA26_BUTTON_H
