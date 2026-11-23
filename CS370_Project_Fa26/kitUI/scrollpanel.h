#ifndef CS370_FA26_SCROLLPANEL_H
#define CS370_FA26_SCROLLPANEL_H

#include "panel.h"
#include "button.h"
#include "rendercontext.h"

struct ScrollPanel : Panel {
    float viewportHeight = 200.0f;
    float scrollOffset = 0.0f;
    float contentHeight = 0.0f;
    float maxScroll = 0.0f;
    float scrollbarWidth = 12.0f;
    Button* thumb = nullptr;

    ScrollPanel(float w, float h, float viewH = 200.0f);
    ScrollPanel(float x, float y, float w, float h, float viewH = 200.0f);
    ~ScrollPanel() override;

    void layout() override;
    void place_thumb();
    void drag_thumb_to(float my);

    bool hit(float mx, float my) const override;
    void on_click(float mx, float my) override;

    void draw_self(const RenderContext& ctx) override;
    void draw_children(const RenderContext& ctx) override;
    void draw_after(const RenderContext& ctx) override;

    void scroll_by(float delta);
};

#endif