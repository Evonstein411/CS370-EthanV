#include "hud.h"

#include <vector>
#include <algorithm>
#include "../common/GLFWutils.h"

HUD& HUD::instance() {
    static HUD hud;
    return hud;
}

void HUD::set_screen_size(int w, int h) {
    screenW  = w;
    screenH = h;
}

int HUD::get_screen_height() {
    return screenH;
}

int HUD::get_screen_width() {
    return screenW;
}

void HUD::register_widget(Widget* w) {
    if (w) widgets.push_back(w);
}

void HUD::destroy_widget(Widget* w) {
    if (!w) return;
    if (handlingEvent) {
        w->pendingDelete = true;
        w->hide();
        trash.push_back(w);
    } else {
        delete w;
    }
}

void HUD::flush_destroyed() {
    for (Widget* w : trash)
        delete w;
    trash.clear();
}

void HUD::draw() {
    //build render context for Widgets
    RenderContext ctx;
    ctx.screenWidth  = screenW;
    ctx.screenHeight = screenH;
    ctx.projMatrix   = make_hud_ortho(screenW, screenH);
    ctx.cameraMatrix = vmath::mat4::identity();

    //update GL state
    begin_hud_pass();

    //sort widgets by priority
    std::vector<Widget*> topLevel;
    for (Widget* w : widgets) {
        if (w && !w->parent && !w->is_effectively_hidden())
            topLevel.push_back(w);
    }
    std::stable_sort(topLevel.begin(), topLevel.end(),
        [](Widget* a, Widget* b) { return a->priority < b->priority; });

    //draw
    for (Widget* w : topLevel)
        w->draw(ctx);

    //restore GL state
    end_hud_pass();
}


void HUD::begin_hud_pass() const {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    // NOTE: IF YOU WANT TRANSPARENCY OF HUD ELEMENTS MAKE SURE ALPHA BLENDING IS ON
}

void HUD::end_hud_pass() const {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}


Widget* HUD::find_topmost(Widget* w, float mx, float my) {
    if (!w || w->is_effectively_hidden() || !w->hit(mx, my))
        return nullptr;

    for (auto it = w->children.rbegin(); it != w->children.rend(); ++it) {
        if (Widget* hit = find_topmost(*it, mx, my))
            return hit;
    }
    return w;
}

Widget* HUD::find_topmost_widget(float mx, float my) {
    Widget* best = nullptr;
    int bestPriority = -1;

    for (Widget* w : widgets) {
        if (!w || w->parent) continue;
        if (Widget* hit = find_topmost(w, mx, my)) {
            if (!best || hit->priority >= bestPriority) {
                best = hit;
                bestPriority = hit->priority;
            }
        }
    }
    return best;
}

void HUD::begin_event() {
    eventDepth++;
    handlingEvent = true;
}

void HUD::end_event() {
    if (--eventDepth == 0) {
        handlingEvent = false;
        flush_destroyed();
    }
}

bool HUD::mouse_button(float mx, float my, int button, int action) {
    begin_event();

    bool consumed = false;
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT) {
        if (Widget* hit = find_topmost_widget(mx, my)) {
            focused = hit;
            hit->on_click(mx, my);
            hit->grab();
            captured = hit;
            consumed = true;
        } else {
            focused = nullptr;
        }
    }
    else if (action == GLFW_RELEASE && button == GLFW_MOUSE_BUTTON_LEFT) {
        if (captured) {
            captured->on_release();
            captured = nullptr;
            consumed = true;
        }
    }

    end_event();
    return consumed;
}

bool HUD::cursor(float mx, float my) {
    for (Widget* w : widgets)
        if (w) w->hovered = false;

    if (Widget* hit = find_topmost_widget(mx, my)) {
        hit->hovered = true;
        hit->on_hover(mx, my);
    }

    if (captured)
        captured->on_drag(mx, my);

    return captured != nullptr;
}

bool HUD::character(unsigned int codepoint) {
    if (focused) {
        focused->on_char(codepoint);
        return true;
    }
    return false;
}

bool HUD::key(int key, int action) {
    begin_event();
    bool consumed = false;
    if (focused) {
        focused->on_key(key, action);
        consumed = true;
    }
    end_event();
    return consumed;
}



