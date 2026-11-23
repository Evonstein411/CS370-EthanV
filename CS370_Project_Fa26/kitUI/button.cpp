#include "button.h"
#include "hud.h"          // for colors / constants if not already visible
#include "rendercontext.h"

Button::Button(float w, float h, std::string label)
    : Widget(w, h), label(std::move(label)) {
    if (prefW <= 0.0f) prefW = this->w;
    if (prefH <= 0.0f) prefH = this->h;
}

void Button::set_label(const std::string& newLabel) {
    label = newLabel;
}

void Button::set_enabled(bool newEnabled) {
    enabled = newEnabled;
}

void Button::fit(const RenderContext& ctx) {
    if (!label.empty()) {
        w = ctx.text_width(label, textSize) + HUD_PAD_X * 2.0f;
        if (w < 48.0f) w = 48.0f;
        h = textSize + HUD_PAD_Y * 2.0f + 4.0f;
        prefW = w;
        prefH = h;
    }
}

void Button::draw_self(const RenderContext& ctx) {
    fit(ctx);

    vmath::mat4 model = vmath::translate(x, y, 0.0f) * vmath::scale(w, h, 1.0f);
    vmath::vec4 drawColor = color;
    if (!enabled)
        drawColor = HUD_DISABLED;
    else if (hovered)
        drawColor = highlightedColor;

    ctx.draw_solid_quad(drawColor, model);
    ctx.draw_string(label, x + HUD_PAD_X, y + HUD_PAD_Y, textSize);
}

void Button::on_click(float /*mx*/, float /*my*/) {
    if (enabled && on_press)
        on_press();
}