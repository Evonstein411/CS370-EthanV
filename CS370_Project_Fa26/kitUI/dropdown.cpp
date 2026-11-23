#include "dropdown.h"
#include "hud.h"
#include <algorithm>

Dropdown::Dropdown(float w, float h, std::string label,
                   const std::vector<std::string>& options, int* selected)
    : Widget(w, h), label(std::move(label)), options(options), selected(selected) {}

Dropdown::Dropdown(float x, float y, float w, float h, std::string label,
                   const std::vector<std::string>& options, int* selected)
    : Widget(w, h), label(std::move(label)), options(options), selected(selected) {
    this->x = x;
    this->y = y;
}

Dropdown::~Dropdown() {
    delete popup;
}

void Dropdown::set_enabled(bool on) {
    enabled = on;
    if (!enabled) close_popup();
}

void Dropdown::set_options(const std::vector<std::string>& newOptions) {
    options = newOptions;
    if (popup) {
        for (auto& cell : popup->cells) {
            if (cell.widget) {
                popup->remove_child(cell.widget);
                delete cell.widget;
            }
        }
        popup->cells.clear();
        // rebuild will happen on next open/draw when we have a context
    }
}

bool Dropdown::contains(float mx, float my) const {
    if (hit(mx, my)) return true;
    if (popup && !popup->hidden && popup->hit(mx, my)) return true;
    return false;
}

void Dropdown::resize(const RenderContext& ctx) {
    labelW = ctx.text_width(label, textSize) + HUD_PAD_X * 2.0f;
    if (labelW < 48.0f) labelW = 48.0f;

    float maxOpt = 0.0f;
    for (const auto& o : options)
        maxOpt = std::max(maxOpt, ctx.text_width(o, textSize));

    float boxW = maxOpt + HUD_PAD_X * 2.0f + 24.0f;
    if (boxW < 100.0f) boxW = 100.0f;

    w = labelW + boxW + HUD_PAD_X;
    h = textSize + HUD_PAD_Y * 2.0f + 4.0f;
    prefW = w;
    prefH = h;
}

std::string Dropdown::currentText() const {
    if (!selected || *selected < 0 || *selected >= static_cast<int>(options.size()))
        return "(none)";
    return options[*selected];
}

void Dropdown::build_options(const RenderContext& ctx) {
    if (!popup) return;

    float maxOpt = 0.0f;
    for (const auto& o : options)
        maxOpt = std::max(maxOpt, ctx.text_width(o, textSize));

    float boxW = maxOpt + HUD_PAD_X * 2.0f + 24.0f;
    if (boxW < 100.0f) boxW = 100.0f;

    for (int i = 0; i < static_cast<int>(options.size()); ++i) {
        auto* b = new Button(boxW, 0.0f, options[i]);
        b->on_press = [this, i]() {
            if (selected) *selected = i;
            close_popup();
        };
        b->prefW = boxW;
        popup->add(b, 0, i, 1, 1, Alignment::STRETCH, Alignment::START);
    }
    popup->layout();
}

void Dropdown::draw_self(const RenderContext& ctx) {
    resize(ctx);   // always keep size correct

    // background
    vmath::mat4 model = vmath::translate(x, y, 0.0f) * vmath::scale(w, h, 1.0f);
    ctx.draw_solid_quad(color, model);
    ctx.draw_string(label, x + HUD_PAD_X, y + HUD_PAD_Y, textSize);

    // value box
    float boxX = x + labelW;
    float boxW = w - labelW - HUD_PAD_X;
    vmath::mat4 boxModel = vmath::translate(boxX, y + 4.0f, 0.0f) * vmath::scale(boxW, h - 8.0f, 1.0f);
    ctx.draw_solid_quad(enabled ? selectedColor : disabledColor, boxModel);

    vmath::vec4 textColor = enabled ? ALT_TEXT_COLOR : DISABLED_TEXT_COLOR;
    ctx.draw_string(currentText(), boxX + 6.0f, y + 6.0f, textSize, textColor);
    ctx.draw_string(open ? "^" : "v", boxX + boxW - 22.0f, y + 6.0f, textSize, textColor);
}

void Dropdown::open_popup(const RenderContext& ctx) {
    if (!popup) {
        popup = new Panel(0.0f, 0.0f);
        popup->set_priority(1);
        build_options(ctx);
    }

    float boxW = w - labelW - HUD_PAD_X;
    popup->x = x + labelW;
    popup->y = y + h;
    popup->w = boxW;
    popup->stretched = true;
    popup->layout();
    popup->show();
    open = true;
}

void Dropdown::close_popup() {
    if (popup) popup->hide();
    open = false;
}

void Dropdown::on_click(float /*mx*/, float /*my*/) {
    if (!enabled) return;
    if (!open) {
        // we don't have a context here; the next draw will finish setup
        open = true;
    } else {
        close_popup();
    }
}


//
// Created by ethan on 11/23/2026.
//
