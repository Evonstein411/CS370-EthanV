#ifndef KITUI_HUD_H
#define KITUI_HUD_H

#include "widget.h"

void register_widget(Widget* w);
void destroy_widget(Widget* w);
void flush_destroyed();

void draw_widgets();
bool hud_mouse_button(float mx, float my, int button, int action);
bool hud_cursor(float mx, float my);
bool hud_char(unsigned int codepoin);
bool hud_key(int key, int action);

#endif
