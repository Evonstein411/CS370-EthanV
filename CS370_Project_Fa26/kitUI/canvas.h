//
// Created by ethan on 11/2/2026.
//

#ifndef CS370_FA26_CANVAS_H
#define CS370_FA26_CANVAS_H

struct Canvas : Widget {
    bool drawBackground = true;
    vec4 color = HUD_PANEL;

    Canvas(float x, float y, float w, float h)
        : Widget(x, y, w, h) {}

    // place a child at a local offset inside the canvas
    void add(Widget* item, float localX, float localY) {
        if (!item) return;
        item->x = x + localX;
        item->y = y + localY;
        add_child(item);
    }

    void set_bounds(float nx, float ny, float nw, float nh) {
        x = nx; y = ny; w = nw; h = nh;
    }

    void draw_self() override {
        if (!drawBackground) return;
        modelMatrix = translate(x, (float)hh - h - y, 0.0f) * scale(w, h, 1.0f);
        draw_solid_hud_quad(color);
    }
};


#endif //CS370_FA26_CANVAS_H
