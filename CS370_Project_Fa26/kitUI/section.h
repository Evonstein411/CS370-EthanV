#ifndef CS370_FA26_SECTION_H
#define CS370_FA26_SECTION_H

#include "widget.h"
#include "panel.h"
#include "rendercontext.h"
#include <string>

struct Section : Widget {
    std::string label;
    bool collapsed = true;
    Panel* content = nullptr;
    float headerH = 0.0f;
    vmath::vec4 color = HUD_SECTION;
    vmath::vec4 highlightedColor = HUD_HIGHLIGHTED;

    Section(float w, float h, std::string label);
    Section(float x, float y, float w, float h, std::string label);
    ~Section() override;

    bool hit(float mx, float my) const override;
    void add(Widget* w);

    void layout(const RenderContext& ctx);
    void draw_self(const RenderContext& ctx) override;
    void on_click(float mx, float my) override;
};

#endif