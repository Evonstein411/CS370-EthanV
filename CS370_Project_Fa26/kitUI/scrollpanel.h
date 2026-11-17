#ifndef CS370_FA26_SCROLLPANEL_H
#define CS370_FA26_SCROLLPANEL_H

#include "widget.h"

struct ScrollPanel : Panel {
    float viewportHeight = 200.0f;   // visible height
    float scrollOffset = 0.0f;
    float contentHeight = 0.0f;
    float maxScroll = 0.0f;
    float scrollbarWidth = 12.0f;
    Button* thumb = nullptr;

    ScrollPanel(const float x, const float y, const float viewH = 200.0f)
        : Panel(x, y), viewportHeight(viewH) {
        h = viewportHeight;
        w = 280.0f;

        thumb = new Button(0, 0, scrollbarWidth - 2.0f, 20.0f, [] {}, "");
        thumb->color = vec4(0.55f, 0.55f, 0.55f, 1.0f);
        thumb->highlightedColor = vec4(0.85f, 0.85f, 0.85f, 1.0f);
        add_child(thumb);   // hit-test + hide with parent; not a layout cell

        thumb->on_press = [this] { thumb->grab(); };
        thumb->on_drag_callback = [this](const float mx, const float my) { drag_thumb_to(my); };
    }



    void layout() override{
        // Run normal panel layout first (computes full content size and positions)
        const float assignedW = w;

        Panel::layout();
        contentHeight = h;
        maxScroll = std::max(0.0f, contentHeight - viewportHeight);
        scrollOffset = std::clamp(scrollOffset, 0.0f, maxScroll);

        // Apply scroll offset to children (shift them up)
        for (const auto& cell : cells) {
            if (cell.widget)
                cell.widget->y -= scrollOffset;
        }

        h = viewportHeight;
        w = assignedW;   // scrollbar stays inside the assigned width

        place_thumb();
    }

    void place_thumb() const {
        if (!thumb) return;
        if (maxScroll <= 0.0f) { thumb->hide(); return; }
        thumb->show();

        const float ratio = viewportHeight / contentHeight;
        thumb->w = scrollbarWidth - 2.0f;
        thumb->h = std::max(20.0f, h * ratio);
        thumb->x = x + w - scrollbarWidth + 1.0f;

        const float travel = h - thumb->h;
        const float thumbY = scrollOffset / maxScroll * travel;
        thumb->y = y + thumbY;
    }

    void drag_thumb_to(const float my) {
        if (!thumb || !thumb->grabbed || maxScroll <= 0.0f) return;
        const float bottom = static_cast<float>(hh) - h - y;
        const float travel = h - thumb->h;
        if (travel <= 0.0f) return;

        const float rel = 1.0f - (my - bottom - thumb->h * 0.5f) / travel;
        scrollOffset = std::clamp(rel * maxScroll, 0.0f, maxScroll);
        place_thumb();
    }

    bool hit(const float mx, const float my) const override {
        if (hidden || is_effectively_hidden()) return false;
        const float bottom = static_cast<float>(hh) - h - y;
        return mx >= x && mx <= x + w &&
               my >= bottom && my <= bottom + h;
    }

    void on_click(const float mx, const float my) override {
        if (thumb && thumb->hit(mx, my))
            return;
        const float bottom = static_cast<float>(hh) - h - y;
        if (const float trackX = x + w - scrollbarWidth; mx >= trackX && maxScroll > 0.0f) {
            const float rel =1.0f - (my - bottom) / h;
            scrollOffset = std::clamp(rel * maxScroll, 0.0f, maxScroll);
            place_thumb();
        }
    }


    void draw_self() override {
        layout();
        const float contentW = w - (maxScroll > 0.0f ? scrollbarWidth : 0.0f);
        modelMatrix = translate(x, static_cast<float>(hh) - h - y, 0.0f) * scale(contentW, h, 1.0f);
        draw_solid_hud_quad(color);

        const int sx = static_cast<int>(x);
        const int sy = static_cast<int>(static_cast<float>(hh) - h - y);
        glEnable(GL_SCISSOR_TEST);
        glScissor(sx, sy, static_cast<int>(contentW), static_cast<int>(h));
    }

    void draw_children() override {
        for (Widget* c : children)
            if (c != thumb)
                c->draw();
    }


    void draw_after() override {
        glDisable(GL_SCISSOR_TEST);
        if (maxScroll > 0.0f) {
            const float bottom = static_cast<float>(hh) - h - y;
            const float trackX = x + w - scrollbarWidth;
            modelMatrix = translate(trackX, bottom, 0.0f) * scale(scrollbarWidth, h, 1.0f);
            draw_solid_hud_quad(vec4(0.2f, 0.2f, 0.2f, 0.9f));
        }
        if (thumb && !thumb->hidden)
            thumb->draw();
    }

    void scroll_by(const float delta) {
        scrollOffset = std::clamp(scrollOffset - delta, 0.0f, maxScroll);
        place_thumb();
    }
};


#endif //CS370_FA26_SCROLLPANEL_H
