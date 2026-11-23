#ifndef CS370_FA26_CANVAS_H
#define CS370_FA26_CANVAS_H

#include "widget.h"
#include "rendercontext.h"

struct Canvas : Widget {
    bool drawBackground = true;
    vmath::vec4 color = HUD_PANEL;

    Canvas(float w, float h);
    Canvas(float x, float y, float w, float h);   // convenience

    void add(Widget* item, float localX, float localY);
    void set_bounds(float nx, float ny, float nw, float nh);

    void draw_self(const RenderContext& ctx) override;
};

#endif