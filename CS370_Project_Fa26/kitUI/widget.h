#ifndef CS370_FA26_WIDGET_H
#define CS370_FA26_WIDGET_H

#include <functional>


struct Widget;

//list of widgets
inline vector<Widget*> widgets;
inline vector<Widget*> trash;
inline bool handlingEvent = false;
inline int eventDepth = 0;
//HUD globals

//colors
const vec4 HUD_PANEL       = vec4(0.28f, 0.22f, 0.16f, 0.92f);  // dark sand / panel bg
const vec4 HUD_FIELD       = vec4(0.38f, 0.30f, 0.22f, 0.95f);  // text fields, dropdowns
const vec4 HUD_CANVAS        = vec4(0.22f, 0.17f, 0.12f, 0.95f);  // top bar, slightly darker
const vec4 HUD_BUTTON      = vec4(0.55f, 0.38f, 0.22f, 0.95f);  // terracotta buttons
const vec4 HUD_DROPFIELD    = vec4(0.45f, 0.34f, 0.22f, 0.95f);  // dropdown label + closed box
const vec4 HUD_SECTION     = vec4(0.36f, 0.27f, 0.18f, 0.95f);  // section bar (normal)
const vec4 HUD_LABEL     = vec4(0.50f, 0.34f, 0.20f, 0.95f);  // section bar (normal)


const vec4 HUD_ACCENT      = vec4(0.85f, 0.55f, 0.25f, 1.00f);  // selected, focus ring
const vec4 HUD_HIGHLIGHTED = vec4(0.75f, 0.55f, 0.30f, 0.95f);  // hover / active
const vec4 HUD_DISABLED    = vec4(0.22f, 0.20f, 0.18f, 0.65f);  // muted


const float HUD_FONT = 32.0f;
const float HUD_PAD_X = 8.0f;
const float HUD_PAD_Y = 8.0f;
const vec4 TEXT_COLOR = vec4(0.95f, 0.90f, 0.82f, 1.00f);
const vec4 ALT_TEXT_COLOR = vec4(0.78f, 0.66f, 0.54f, 1.00f);
const vec4 DISABLED_TEXT_COLOR = vec4(0.24, 0.22f, 0.20f, 1.0f);

enum class Alignment {START, CENTER, END, STRETCH};
//parent HUD element struct
struct Widget {
    float x, y, w, h;
    float prefW = 0.0f;
    float prefH = 0.0f;
    bool hovered = false;
    bool hidden = false;
    bool grabbed = false;
    bool pendingDelete = false;
    bool stretched = false;
    int priority = 0; //higher priority value drawn first

    std::function<void(float, float)> on_drag_callback;
    std::function<void()> on_release_callback;

    vector<Widget*> children; //list of all current children
    Widget* parent = nullptr; //pointer to parent if have one

    Widget(float x, float y, float w, float h)
        : x(x), y(y), w(w), h(h) {
        widgets.push_back(this);
    }

    virtual ~Widget() {
        auto it = std::find(widgets.begin(), widgets.end(), this);
        if (it != widgets.end())
            widgets.erase(it);

        if (parent) parent->remove_child(this); //if im a child remove myself from parent
        for (Widget* c : children) c->parent = nullptr; //clear self from parent of all children
        children.clear(); //clear children list

    }

    virtual void draw_self() = 0; //defined per widget
    virtual void draw_after(){}
    virtual void on_char(unsigned int) {}
    virtual void on_key(int key, int action) {}
    virtual void on_click(float mx, float my) {}
    virtual void on_hover(float mx, float my){}

    virtual bool hit(float mx, float my) const {
        if (hidden || is_effectively_hidden()) return false;
        float bottom = (float)hh - h - y;
        return mx >= x && mx <= x + w && my >= bottom && my <= bottom + h;
    }

    //function for drawing children
    virtual void draw_children() {
        for (Widget* c : children) {
            c->draw();
        }
    }

    void mark_for_delete() {
        if (pendingDelete) return;
        pendingDelete = true;
        hidden = true;
    }

    //if hidden skip else draws self, draws children, and finally draws after for any drawing that needs to be done after children
    void draw() {
        if(hidden) {
            return;
        }
        draw_self();
        draw_children();
        draw_after();

    }

    //interaction

    virtual void on_drag(float mx, float my) {
        if (on_drag_callback) {on_drag_callback(mx, my);}
    }

    virtual void on_release() {
        grabbed = false;
        if (on_release_callback) on_release_callback();
    }

    void grab() {
        grabbed = true;
    }


    //parent child relations
    Widget* get_parent() {
        return parent;
    }

    void add_child(Widget* child) {
        if(child == this) return; //cant child self

        //remove from any prior parent
        if (child->is_child()) {
            child->get_parent()->remove_child(child);
        }

        child->parent = this;
        children.push_back(child);
    }

    void remove_child(Widget* child) {
        if(!child) return; //do nothing if child doesn't exist
        auto searched_child = std::find(children.begin(), children.end(), child);
        if (searched_child == children.end()) return; //failed to find child do nothing
        children.erase(searched_child); //found child remove from children list
        if (child->get_parent() == this) { //remove self from parent of prior child
            child->parent = nullptr;
        }
    }


    //only remove and add child directly write to parent to prevent desync of child and parent status
    void set_parent(Widget* new_parent) {
        if(new_parent == parent) return; //do nothing if already parent
        if(new_parent) {
            new_parent->add_child(this);   //add to new parent
        }
        else if(parent) {
            parent->remove_child(this);
        }
    }

    bool is_child() const {
        return parent != nullptr;
    }


    //visibility status
    void set_priority(int newPriority) {
        priority = newPriority;
    }

    bool is_hidden() {
        return hidden;
    }

    bool is_effectively_hidden() const {
        for (const Widget* p = parent; p != nullptr; p = p->parent) {
            if (p->hidden) return true;
        }
        return false;
    }

    void hide() {
        hidden = true;
    }

    void show() {
        hidden = false;
    }

    void set_hidden(bool new_hidden) {
        hidden = new_hidden;
    }


}; //end Widget


//widget deletion utils
inline void destroy_widget(Widget* w) {
    if (!w || w->pendingDelete) return;
    if (handlingEvent) {
        w->mark_for_delete();
        trash.push_back(w);
    } else {
        delete w;
    }
}

inline void flush_destroyed() {
    if (trash.empty()) return;
    vector<Widget*> batch;
    batch.swap(trash);
    for (Widget* w : batch)
        delete w;                      // destructor already unregisters from widgets
}

//place this object in any callback/event to cleanly handle deletion
struct EventScope {
    EventScope() { eventDepth++; handlingEvent = true; }
    ~EventScope() {
        if (--eventDepth == 0) {
            handlingEvent = false;
            flush_destroyed();
        }
    }
};

#endif //CS370_FA26_WIDGET_H
