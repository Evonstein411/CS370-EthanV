#ifndef CS370_FA26_BUTTON_H
#define CS370_FA26_BUTTON_H

#include "widget.h"
#include "hud.h"
#include <functional>
#include <string>
#include "../common/vmath.h"

struct Button : Widget {
    std::string label;
    float textSize = HUD_FONT;
    std::function<void()> on_press;
    vmath::vec4 color = HUD_BUTTON;
    vmath::vec4 highlightedColor = HUD_HIGHLIGHTED;
    bool enabled = true;

    Button(float w, float h, std::string label = "");
    void set_label(const std::string& newLabel);
    void set_enabled(bool newEnabled);
    void fit(const RenderContext& ctx);

    void draw_self(const RenderContext& ctx) override;
    void on_click(float mx, float my) override;
};

#endif // CS370_FA26_BUTTON_H