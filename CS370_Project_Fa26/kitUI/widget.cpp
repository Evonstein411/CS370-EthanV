#include "widget.h"

#include <algorithm>
#include <cstdio>


Widget::Widget(float w, float h)
    : w(w), h(h)
{
    widgets.push_back(this);
}

Widget::~Widget() {
    // remove from global list
    auto it = std::find(widgets.begin(), widgets.end(), this);
    if (it != widgets.end())
        widgets.erase(it);

    // detach from parent
    if (parent)
        parent->remove_child(this);

    // detach children (do not delete them)
    for (Widget* c : children)
        if (c) c->parent = nullptr;
    children.clear();
}

bool Widget::hit(float mx, float my) const {
    if (hidden || is_effectively_hidden()) return false;
    return mx >= x && mx <= x + w && my >= y && my <= y + h;
}

void Widget::draw_children() {
    for (Widget* c : children)
        if (c) c->draw();
}

void Widget::draw() {
    if (hidden || is_effectively_hidden()) return;
    draw_self();
    draw_children();
    draw_after();
}

void Widget::mark_for_delete() {
    pendingDelete = true;
    hidden = true;
    trash.push_back(this);
}

void Widget::on_drag(float mx, float my) {
    if (on_drag_callback)
        on_drag_callback(mx, my);
}

void Widget::on_release() {
    grabbed = false;
    if (on_release_callback)
        on_release_callback();
}

void Widget::grab() {
    grabbed = true;
}

void Widget::add_child(Widget* child) {
    if (!child || child == this) return;

    if (child->parent)
        child->parent->remove_child(child);

    child->parent = this;
    children.push_back(child);
}

void Widget::remove_child(Widget* child) {
    if (!child) return;
    auto it = std::find(children.begin(), children.end(), child);
    if (it == children.end()) return;
    children.erase(it);
    child->parent = nullptr;
}

void Widget::set_parent(Widget* new_parent) {
    if (parent == new_parent) return;
    if (parent)
        parent->remove_child(this);
    if (new_parent)
        new_parent->add_child(this);
}

bool Widget::is_effectively_hidden() const {
    for (const Widget* p = parent; p; p = p->parent)
        if (p->hidden) return true;
    return false;
}

void destroy_widget(Widget* w) {
    if (!w) return;
    if (handlingEvent)
        w->mark_for_delete();
    else
        delete w;
}

void flush_destroyed() {
    for (Widget* w : trash)
        delete w;
    trash.clear();
}

