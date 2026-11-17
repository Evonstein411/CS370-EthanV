#ifndef CS370_FA26_CANVAS_H
#define CS370_FA26_CANVAS_H

#include "widget.h"


struct Canvas : Widget {
    bool drawBackground = true;
    vec4 color = HUD_PANEL;

    Canvas(const float x, const float y, const float w, const float h)
        : Widget(x, y, w, h) {}

    // place a child at a local offset inside the canvas
    void add(Widget* item, const float localX, const float localY) {
        if (!item) return;
        item->x = x + localX;
        item->y = y + localY;
        add_child(item);
    }

    void set_bounds(const float nx, const float ny, const float nw, const float nh) {
        x = nx; y = ny; w = nw; h = nh;
    }

    void draw_self() override {
        if (!drawBackground) return;
        modelMatrix = translate(x, static_cast<float>(hh) - h - y, 0.0f) * scale(w, h, 1.0f);
        draw_solid_hud_quad(color);
    }
};


#endif //CS370_FA26_CANVAS_H
