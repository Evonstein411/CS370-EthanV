#include "label.h"
#include "hud.h"

Label::Label(float w, float h, std::string label)
    : Widget(w, h), label(std::move(label)) {
    if (prefW <= 0.0f) prefW = w;
    if (prefH <= 0.0f) prefH = h;
}


void Label::set_text(const std::string& newText) {
    label = newText;
}

void Label::fit(const RenderContext& ctx) {
    w = ctx.text_width(label, textSize) + HUD_PAD_X * 2.0f;
    h = textSize + HUD_PAD_Y * 2.0f + 4.0f;
    prefW = w;
    prefH = h;
}

void Label::draw_self(const RenderContext& ctx) {
    fit(ctx);

    // top-left origin
    vmath::mat4 model = vmath::translate(x, y, 0.0f) * vmath::scale(w, h, 1.0f);
    ctx.draw_solid_quad(color, model);
    ctx.draw_string(label, x + HUD_PAD_X, y + HUD_PAD_Y, textSize);
}