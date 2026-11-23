#ifndef CS370_FA26_WIDGET_H
#define CS370_FA26_WIDGET_H

#include <vector>
#include <functional>

#include "rendercontext.h"

enum class Alignment {
    START,
    CENTER,
    END,
    STRETCH
};


struct Widget {
    float x = 0, y = 0, w = 0, h = 0;
    float prefW = 0, prefH = 0;
    bool hidden = false;
    bool hovered = false;
    bool grabbed = false;
    bool stretched = false;
    bool pendingDelete = false;
    int priority = 0;

    std::function<void(float, float)> on_drag_callback;
    std::function<void()> on_release_callback;

    std::vector<Widget*> children;
    Widget* parent = nullptr;

    explicit Widget(float w, float h);
    virtual ~Widget();

    void draw(const RenderContext& ctx);
    virtual void draw_self(const RenderContext& ctx) = 0;
    virtual void draw_children(const RenderContext& ctx);
    virtual void draw_after(const RenderContext& ctx) {}


    virtual void on_char(unsigned int) {}
    virtual void on_key(int key, int action) {}
    virtual void on_click(float mx, float my) {}
    virtual void on_hover(float mx, float my) {}

    virtual bool hit(float mx, float my) const;

    void mark_for_delete();
    virtual void on_drag(float mx, float my);
    virtual void on_release();
    void grab();

    Widget* get_parent() const { return parent; }
    void add_child(Widget* child);
    void remove_child(Widget* child);
    void set_parent(Widget* new_parent);
    bool is_child() const { return parent != nullptr; }

    void set_priority(int p) { priority = p; }
    bool is_hidden() const { return hidden; }
    bool is_effectively_hidden() const;
    void hide() { hidden = true; }
    void show() { hidden = false; }
    void set_hidden(bool newHidden) { hidden = newHidden; }
};

#endif //CS370_FA26_WIDGET_H
