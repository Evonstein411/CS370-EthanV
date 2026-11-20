#ifndef KITUI_HUD_H
#define KITUI_HUD_H

#include "widget.h"
#include "../common/vmath.h"
#include <vector>

//colors
const auto HUD_PANEL = vmath::vec4(0.28f, 0.22f, 0.16f, 0.92f);  // dark sand / panel bg
const auto HUD_FIELD = vmath::vec4(0.38f, 0.30f, 0.22f, 0.95f);  // text fields, dropdowns
const auto HUD_CANVAS = vmath::vec4(0.22f, 0.17f, 0.12f, 0.95f);  // top bar, slightly darker
const auto HUD_BUTTON = vmath::vec4(0.55f, 0.38f, 0.22f, 0.95f);  // terracotta buttons
const auto HUD_DROPFIELD = vmath::vec4(0.45f, 0.34f, 0.22f, 0.95f);  // dropdown label + closed box
const auto HUD_SECTION = vmath::vec4(0.36f, 0.27f, 0.18f, 0.95f);  // section bar (normal)
const auto HUD_LABEL = vmath::vec4(0.50f, 0.34f, 0.20f, 0.95f);  // section bar (normal)
const auto HUD_ACCENT      = vmath::vec4(0.85f, 0.55f, 0.25f, 1.00f);  // selected, focus ring
const auto HUD_HIGHLIGHTED = vmath::vec4(0.75f, 0.55f, 0.30f, 0.95f);  // hover / active
const auto HUD_DISABLED    = vmath::vec4(0.22f, 0.20f, 0.18f, 0.65f);  // muted
const auto ALT_TEXT_COLOR = vmath::vec4(0.78f, 0.66f, 0.54f, 1.00f);
const auto DISABLED_TEXT_COLOR = vmath::vec4(0.24, 0.22f, 0.20f, 1.0f);
const auto TEXT_COLOR = vmath::vec4(0.95f, 0.90f, 0.82f, 1.00f);

constexpr float HUD_FONT = 32.0f;
constexpr float HUD_PAD_X = 8.0f;
constexpr float HUD_PAD_Y = 8.0f;

class HUD {
public:
    static HUD& instance();

    void register_widget(Widget* w);
    void destroy_widget(Widget* w);
    void flush_destroyed();

    void draw();

    bool mouse_button(float mx, float my, int button, int action);
    bool cursor(float mx, float my);
    bool character(unsigned int codepoint);
    bool key(int key, int action);



private:
    HUD() = default;

    std::vector<Widget*> widgets;
    std::vector<Widget*> trash;
    Widget* captured = nullptr;
    Widget* focused = nullptr;
    bool handlingEvent = false;
    int eventDepth = 0;

    Widget* find_topmost(Widget* w, float mx, float my);
    Widget* find_topmost_widget(float mx, float my);
    void begin_event();
    void end_event();

};



#endif //KITUI_HUD_H
