#include "section.h"
#include "hud.h"
#include <algorithm>

Section::Section(float w, float h, std::string label)
    : Widget(w, h), label(std::move(label)) {
    headerH = HUD_FONT + HUD_PAD_Y * 2.0f;
    content = new Panel(0.0f, 0.0f);
    content->set_hidden(collapsed);
    add_child(content);
}

Section::Section(float x, float y, float w, float h, std::string label)
    : Section(w, h, std::move(label)) {
    this->x = x;
    this->y = y;
}

Section::~Section() {
    delete content;
}

bool Section::hit(float mx, float my) const {
    if (hidden || is_effectively_hidden()) return false;
    // only the header is interactive (top-left origin)
    return mx >= x && mx <= x + w &&
           my >= y && my <= y + headerH;
}

void Section::add(Widget* w) {
    if (content)
        content->add(w, 0, 0);
}

void Section::layout(const RenderContext& ctx) {
    const float assignedW = stretched ? w : 0.0f;
    stretched = false;
    headerH = HUD_FONT + HUD_PAD_Y * 2.0f;

    const float labelW = ctx.text_width(label, HUD_FONT) + HUD_PAD_X * 3.0f + 20.0f;

    if (collapsed || !content || content->cells.empty()) {
        prefW = std::max(labelW, 48.0f);
        w = std::max(prefW, assignedW);
        h = headerH;
        return;
    }

    content->x = x;
    content->y = y + headerH;
    content->layout();

    prefW = std::max(content->w, labelW);
    w = std::max(prefW, assignedW);
    h = headerH + content->h;
    content->w = w;
    content->stretched = true;
}

void Section::draw_self(const RenderContext& ctx) {
    layout(ctx);

    // header bar
    vmath::mat4 model = vmath::translate(x, y, 0.0f) * vmath::scale(w, headerH, 1.0f);
    ctx.draw_solid_quad(hovered ? highlightedColor : color, model);

    const std::string arrow = collapsed ? ">" : "v";
    ctx.draw_string(arrow, x + HUD_PAD_X, y + HUD_PAD_Y, HUD_FONT);
    ctx.draw_string(label, x + HUD_PAD_X + 20.0f, y + HUD_PAD_Y, HUD_FONT);
}

void Section::on_click(float /*mx*/, float /*my*/) {
    collapsed = !collapsed;
    if (content) content->set_hidden(collapsed);
}