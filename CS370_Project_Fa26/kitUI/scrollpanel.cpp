#include "scrollpanel.h"
#include "hud.h"
#include "../common/GLFWutils.h"
#include <algorithm>

ScrollPanel::ScrollPanel(float w, float h, float viewH)
    : Panel(w, h), viewportHeight(viewH) {
    this->h = viewportHeight;

    thumb = new Button(scrollbarWidth - 2.0f, 20.0f, "");
    thumb->color = vmath::vec4(0.55f, 0.55f, 0.55f, 1.0f);
    thumb->highlightedColor = vmath::vec4(0.85f, 0.85f, 0.85f, 1.0f);
    add_child(thumb);

    thumb->on_press = [this] { thumb->grab(); };
    // on_drag_callback can be re-added once drag events are wired
}

ScrollPanel::ScrollPanel(float x, float y, float w, float h, float viewH)
    : ScrollPanel(w, h, viewH) {
    this->x = x;
    this->y = y;
}

ScrollPanel::~ScrollPanel() {
}

void ScrollPanel::layout() {
    const float assignedW = w;
    Panel::layout();// now calls GridLayout

    contentHeight = h;
    maxScroll = std::max(0.0f, contentHeight - viewportHeight);
    scrollOffset = std::clamp(scrollOffset, 0.0f, maxScroll);

    for (auto& cell : cells)
        if (cell.widget)
            cell.widget->y -= scrollOffset;   // top-left: subtracting moves content up

    h = viewportHeight;
    w = assignedW;
    place_thumb();
}

void ScrollPanel::place_thumb() {
    if (!thumb) return;
    if (maxScroll <= 0.0f) { thumb->hide(); return; }
    thumb->show();

    const float ratio = viewportHeight / contentHeight;
    thumb->w = scrollbarWidth - 2.0f;
    thumb->h = std::max(20.0f, h * ratio);
    thumb->x = x + w - scrollbarWidth + 1.0f;

    const float travel = h - thumb->h;
    const float thumbY = (scrollOffset / maxScroll) * travel;
    thumb->y = y + thumbY;           // top-left
}

void ScrollPanel::drag_thumb_to(float my) {
    if (!thumb || !thumb->grabbed || maxScroll <= 0.0f) return;
    const float travel = h - thumb->h;
    if (travel <= 0.0f) return;

    // top-left origin: my increases downward
    const float rel = (my - y - thumb->h * 0.5f) / travel;
    scrollOffset = std::clamp(rel * maxScroll, 0.0f, maxScroll);
    place_thumb();
}

bool ScrollPanel::hit(float mx, float my) const {
    if (hidden || is_effectively_hidden()) return false;
    return mx >= x && mx <= x + w &&
           my >= y && my <= y + h;   // top-left
}

void ScrollPanel::on_click(float mx, float my) {
    if (thumb && thumb->hit(mx, my)) return;

    if (const float trackX = x + w - scrollbarWidth; mx >= trackX && maxScroll > 0.0f) {
        const float rel = (my - y) / h;
        scrollOffset = std::clamp(rel * maxScroll, 0.0f, maxScroll);
        place_thumb();
    }
}

void ScrollPanel::draw_self(const RenderContext& ctx) {
    layout();

    const float contentW = w - (maxScroll > 0.0f ? scrollbarWidth : 0.0f);
    vmath::mat4 model = vmath::translate(x, y, 0.0f) * vmath::scale(contentW, h, 1.0f);
    ctx.draw_solid_quad(color, model);

    // OpenGL scissor is bottom-left, so convert from top-left widget coords
    const int sx = static_cast<int>(x);
    const int sy = static_cast<int>(ctx.screen_height() - (y + h));
    glEnable(GL_SCISSOR_TEST);
    glScissor(sx, sy, static_cast<int>(contentW), static_cast<int>(h));
}

void ScrollPanel::draw_children(const RenderContext& ctx) {
    for (Widget* c : children)
        if (c && c != thumb)
            c->draw(ctx);
}

void ScrollPanel::draw_after(const RenderContext& ctx) {
    glDisable(GL_SCISSOR_TEST);

    if (maxScroll > 0.0f) {
        const float trackX = x + w - scrollbarWidth;
        vmath::mat4 track = vmath::translate(trackX, y, 0.0f) * vmath::scale(scrollbarWidth, h, 1.0f);
        ctx.draw_solid_quad(vmath::vec4(0.2f, 0.2f, 0.2f, 0.9f), track);
    }

    if (thumb && !thumb->hidden)
        thumb->draw(ctx);
}

void ScrollPanel::scroll_by(float delta) {
    scrollOffset = std::clamp(scrollOffset - delta, 0.0f, maxScroll);
    place_thumb();
}