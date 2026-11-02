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
const vec4 HUD_GRAY = vec4(0.4f, 0.4f, 0.4f, 0.85f);
const vec4 HUD_PANEL = vec4(0.3f, 0.3f, 0.3f, 0.85);
const vec4 HUD_HIGHLIGHTED = vec4(0.8f, 0.8f, 0.8f, 0.85f);
const vec4 HUD_FIELD = vec4(0.9f, 0.9f, 0.9f, 0.85f);
const vec4 HUD_DISABLED = vec4(0.6f, 0.6f, 0.6f, 0.85f);
const float HUD_FONT = 32.0f;
const float HUD_PAD_X = 8.0f;
const float HUD_PAD_Y = 8.0f;
const vec4 TEXT_COLOR = vec4(1.0f, 1.0f, 1.0f, 1.0f);
const vec4 ALT_TEXT_COLOR = vec4(0.2f, 0.2f, 0.2f, 1.0f);

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

    std::function<void(float, float)> onDragCallback;
    std::function<void()> onReleaseCallback;

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

        if (parent) parent->removeChild(this); //if im a child remove myself from parent
        for (Widget* c : children) c->parent = nullptr; //clear self from parent of all children
        children.clear(); //clear children list

    }

    virtual void drawSelf() = 0; //defined per widget
    virtual void drawAfter(){}
    virtual void on_char(unsigned int) {}
    virtual void on_key(int key, int action) {}
    virtual void on_click(float mx, float my) {}
    virtual void on_hover(float mx, float my){}

    virtual bool hit(float mx, float my) const {
        if (hidden || isEffectivelyHidden()) return false;
        float bottom = (float)hh - h - y;
        return mx >= x && mx <= x + w && my >= bottom && my <= bottom + h;
    }

    //function for drawing children
    virtual void drawChildren() {
        for (Widget* c : children) {
            c->draw();
        }
    }

    void markForDelete() {
        if (pendingDelete) return;
        pendingDelete = true;
        hidden = true;
    }

    //if hidden skip else draws self, draws children, and finally draws after for any drawing that needs to be done after children
    void draw() {
        if(hidden) {
            return;
        }
        drawSelf();
        drawChildren();
        drawAfter();

    }

    //interaction

    virtual void on_drag(float mx, float my) {
        if (onDragCallback) {onDragCallback(mx, my);}
    }

    virtual void on_release() {
        grabbed = false;
        if (onReleaseCallback) onReleaseCallback();
    }

    void grab() {
        grabbed = true;
    }


    //parent child relations
    Widget* getParent() {
        return parent;
    }

    void addChild(Widget* child) {
        if(child == this) return; //cant child self

        //remove from any prior parent
        if (child->isChild()) {
            child->getParent()->removeChild(child);
        }

        child->parent = this;
        children.push_back(child);
    }

    void removeChild(Widget* child) {
        if(!child) return; //do nothing if child doesn't exist
        auto searched_child = std::find(children.begin(), children.end(), child);
        if (searched_child == children.end()) return; //failed to find child do nothing
        children.erase(searched_child); //found child remove from children list
        if (child->getParent() == this) { //remove self from parent of prior child
            child->parent = nullptr;
        }
    }


    //only remove and add child directly write to parent to prevent desync of child and parent status
    void setParent(Widget* new_parent) {
        if(new_parent == parent) return; //do nothing if already parent
        if(new_parent) {
            new_parent->addChild(this);   //add to new parent
        }
        else if(parent) {
            parent->removeChild(this);
        }
    }

    bool isChild() const {
        return parent != nullptr;
    }


    //visibility status
    void setPriority(int new_priority) {
        priority = new_priority;
    }

    bool isHidden() {
        return hidden;
    }

    bool isEffectivelyHidden() const {
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

    void setHidden(bool new_hidden) {
        hidden = new_hidden;
    }


}; //end Widget


//widget deletion utils
inline void destroy_widget(Widget* w) {
    if (!w || w->pendingDelete) return;
    if (handlingEvent) {
        w->markForDelete();
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
