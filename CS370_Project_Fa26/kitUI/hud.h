#ifndef KITUI_HUD_H
#define KITUI_HUD_H

#include "widget.h"
#include <vector>



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

    void set_screen_size(int w, int h);
    int get_screen_width();
    int get_screen_height();



private:
    HUD() = default;

    int screenW = 0;
    int screenH = 0;

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

    void begin_hud_pass() const;
    void end_hud_pass() const;

};



#endif //KITUI_HUD_H
