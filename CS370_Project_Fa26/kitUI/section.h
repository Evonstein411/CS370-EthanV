#ifndef CS370_FA26_SECTION_H
#define CS370_FA26_SECTION_H

struct Section : Widget {
    string label;
    bool collapsed = true;
    Panel* content;
    float headerH = 0.0f;
    vec4 color = HUD_SECTION;
    vec4 highlightedColor = HUD_HIGHLIGHTED;

    Section(float x, float y, const string& label)
        : Widget(x, y, 0, 0), label(label) {
        headerH = HUD_FONT + HUD_PAD_Y * 2.0f;
        content = new Panel(x, y);
        content->setHidden(collapsed);
        addChild(content);
    }

    ~Section() {
        delete content;
    }

    bool hit(float mx, float my) const override {
        if (hidden || isEffectivelyHidden()) return false;
        // only the header bar is interactive
        float bottom = (float)hh - headerH - y;
        return mx >= x && mx <= x + w && my >= bottom && my <= bottom + headerH;
    }

    void add(Widget* w) {
        content->add(w, 0, 0);
    }

    void layout() {
        float assignedW = stretched ? w : 0.0f;
        stretched = false;
        headerH = HUD_FONT + HUD_PAD_Y * 2.0f;

        float labelW = text_width(label, HUD_FONT) + HUD_PAD_X * 3.0f + 20.0f;

        if (collapsed || content->cells.empty()) {
            prefW = std::max(labelW, 48.0f);
            w = std::max(prefW, assignedW);
            h = headerH;
            return;
        }

        content->x = x;
        content->y = y + headerH;
        content->layout();

        prefW = std::max(content->w, labelW);
        w = std::max(prefW, assignedW);
        h = headerH + content->h;
        content->w = w;
        content->stretched = true;   // body layout must not shrink this
    }

    void drawSelf() override {
        layout();

        // header bar
        mat4 trans_matrix = translate(x, (float)hh - headerH - y, 0.0f);
        mat4 scale_matrix   = scale(w, headerH, 1.0f);
        model_matrix = trans_matrix * scale_matrix;
        draw_solid_hud_quad(hovered ? highlightedColor : color);

        string arrow = collapsed ? ">" : "v";
        draw_string(arrow, x + HUD_PAD_X, y + HUD_PAD_Y, HUD_FONT);
        draw_string(label, x + HUD_PAD_X + 20.0f, y + HUD_PAD_Y, HUD_FONT);


    }

    void on_click(float mx, float my) override {
        // mouse callback already confirmed we were the top-most hit
        collapsed = !collapsed;
        content->setHidden(collapsed);
        layout();
    }


};//end Section




#endif //CS370_FA26_SECTION_H
