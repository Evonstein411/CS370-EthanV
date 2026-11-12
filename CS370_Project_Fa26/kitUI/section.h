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
        content->set_hidden(collapsed);
        add_child(content);
    }

    ~Section() {
        delete content;
    }

    bool hit(float mx, float my) const override {
        if (hidden || is_effectively_hidden()) return false;
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

    void draw_self() override {
        layout();

        // header bar
        mat4 transMatrix = translate(x, (float)hh - headerH - y, 0.0f);
        mat4 scaleMatrix   = scale(w, headerH, 1.0f);
        modelMatrix = transMatrix * scaleMatrix;
        draw_solid_hud_quad(hovered ? highlightedColor : color);

        string arrow = collapsed ? ">" : "v";
        draw_string(arrow, x + HUD_PAD_X, y + HUD_PAD_Y, HUD_FONT);
        draw_string(label, x + HUD_PAD_X + 20.0f, y + HUD_PAD_Y, HUD_FONT);


    }

    void on_click(float mx, float my) override {
        // mouse callback already confirmed we were the top-most hit
        collapsed = !collapsed;
        content->set_hidden(collapsed);
        layout();
    }


};//end Section




#endif //CS370_FA26_SECTION_H
