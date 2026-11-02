#ifndef CS370_FA26_SCROLLPANEL_H
#define CS370_FA26_SCROLLPANEL_H

struct ScrollPanel : Panel {
    float viewportHeight = 200.0f;   // visible height
    float scrollOffset = 0.0f;
    float contentHeight = 0.0f;
    float maxScroll = 0.0f;
    float scrollbarWidth = 12.0f;
    Button* thumb = nullptr;

    ScrollPanel(float x, float y, float viewH = 200.0f)
        : Panel(x, y), viewportHeight(viewH) {
        h = viewportHeight;
        w = 280.0f;

        thumb = new Button(0, 0, scrollbarWidth - 2.0f, 20.0f, []() {}, "");
        thumb->color = vec4(0.55f, 0.55f, 0.55f, 1.0f);
        thumb->highlightedColor = vec4(0.85f, 0.85f, 0.85f, 1.0f);
        addChild(thumb);   // hit-test + hide with parent; not a layout cell

        thumb->onPress = [this]() { thumb->grab(); };
        thumb->onDragCallback = [this](float mx, float my) { dragThumbTo(my); };
    }



    void layout() override{
        // Run normal panel layout first (computes full content size and positions)
        float assignedW = w;

        Panel::layout();
        contentHeight = h;
        maxScroll = std::max(0.0f, contentHeight - viewportHeight);
        scrollOffset = std::clamp(scrollOffset, 0.0f, maxScroll);

        // Apply scroll offset to children (shift them up)
        for (auto& cell : cells) {
            if (cell.widget)
                cell.widget->y -= scrollOffset;
        }

        h = viewportHeight;
        w = assignedW;   // scrollbar stays inside the assigned width

        placeThumb();
    }

    void placeThumb() {
        if (!thumb) return;
        if (maxScroll <= 0.0f) { thumb->hide(); return; }
        thumb->show();

        float ratio = viewportHeight / contentHeight;
        thumb->w = scrollbarWidth - 2.0f;
        thumb->h = std::max(20.0f, h * ratio);
        thumb->x = x + w - scrollbarWidth + 1.0f;

        float travel = h - thumb->h;
        float thumbY = (scrollOffset / maxScroll) * travel;
        thumb->y = y + thumbY;
    }

    void dragThumbTo(float my) {
        if (!thumb || !thumb->grabbed || maxScroll <= 0.0f) return;
        float bottom = (float)hh - h - y;
        float travel = h - thumb->h;
        if (travel <= 0.0f) return;

        float rel = 1.0f - (my - bottom - thumb->h * 0.5f) / travel;
        scrollOffset = std::clamp(rel * maxScroll, 0.0f, maxScroll);
        placeThumb();
    }

    bool hit(float mx, float my) const override {
        if (hidden || isEffectivelyHidden()) return false;
        float bottom = (float)hh - h - y;
        return mx >= x && mx <= x + w &&
               my >= bottom && my <= bottom + h;
    }

    void on_click(float mx, float my) override {
        if (thumb && thumb->hit(mx, my))
            return;
        float bottom = (float)hh - h - y;
        float trackX = x + w - scrollbarWidth;
        if (mx >= trackX && maxScroll > 0.0f) {
            float rel =1.0f - (my - bottom) / h;
            scrollOffset = std::clamp(rel * maxScroll, 0.0f, maxScroll);
            placeThumb();
        }
    }


    void drawSelf() override {
        layout();
        float contentW = w - (maxScroll > 0.0f ? scrollbarWidth : 0.0f);
        model_matrix = translate(x, (float)hh - h - y, 0.0f) * scale(contentW, h, 1.0f);
        draw_solid_hud_quad(color);

        int sx = (int)x;
        int sy = (int)((float)hh - h - y);
        glEnable(GL_SCISSOR_TEST);
        glScissor(sx, sy, (int)contentW, (int)h);
    }

    void drawChildren() override {
        for (Widget* c : children)
            if (c != thumb)
                c->draw();
    }


    void drawAfter() override {
        glDisable(GL_SCISSOR_TEST);
        if (maxScroll > 0.0f) {
            float bottom = (float)hh - h - y;
            float trackX = x + w - scrollbarWidth;
            model_matrix = translate(trackX, bottom, 0.0f) * scale(scrollbarWidth, h, 1.0f);
            draw_solid_hud_quad(vec4(0.2f, 0.2f, 0.2f, 0.9f));
        }
        if (thumb && !thumb->hidden)
            thumb->draw();
    }

    void scrollBy(float delta) {
        scrollOffset = std::clamp(scrollOffset - delta, 0.0f, maxScroll);
        placeThumb();
    }
};


#endif //CS370_FA26_SCROLLPANEL_H
