//
// Created by ethan on 11/23/2026.
//
#include "canvas.h"
#include "hud.h"

Canvas::Canvas(float w, float h)
    : Widget(w, h) {}

Canvas::Canvas(float x, float y, float w, float h)
    : Widget(w, h) {
    this->x = x;
    this->y = y;
}

void Canvas::add(Widget* item, float localX, float localY) {
    if (!item) return;
    item->x = x + localX;
    item->y = y + localY;
    add_child(item);
}

void Canvas::set_bounds(float nx, float ny, float nw, float nh) {
    x = nx;
    y = ny;
    w = nw;
    h = nh;
}

void Canvas::draw_self(const RenderContext& ctx) {
    if (!drawBackground) return;

    vmath::mat4 model = vmath::translate(x, y, 0.0f) * vmath::scale(w, h, 1.0f);
    ctx.draw_solid_quad(color, model);
}