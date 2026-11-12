#ifndef CS370_FA26_DROPDOWN_H
#define CS370_FA26_DROPDOWN_H
#include "widget.h"


struct Dropdown : Widget {
    string label;
    vector<string> options;
    int* selected = nullptr;   // binds to an int (index into options)
    bool open = false;
    bool closing = false;
    bool enabled = true;
    float textSize = HUD_FONT;
    float labelW = 0.0f;
    vec4 color = HUD_DROPFIELD;
    vec4 highlightedColor = HUD_HIGHLIGHTED;
    vec4 selectedColor = HUD_FIELD;
    vec4 disabledColor = HUD_DISABLED;

    Panel* popup = nullptr;


    //int index base
    Dropdown(float x, float y, float w, float h,
                 const string& label,
                 const vector<string>& options,
                 int* selected)
            : Widget(x, y, w, h), label(label), options(options), selected(selected)
    {
        resize();
    }

    //for passing in enums directly
    template<typename E>
    Dropdown(float x, float y, float w, float h,
             const string& label,
             const vector<string>& options,
             E& selectedEnum)
        : Dropdown(x, y, w, h, label, options, reinterpret_cast<int*>(&selectedEnum))
    {
        resize();
    }

    ~Dropdown() {
        delete popup;
    }

    void set_enabled(bool on) {
        enabled = on;
        if (!enabled)
            close_popup();
    }

    void set_options(const vector<string>& newOptions) {
        options = newOptions;
        //guard if no popup
        if (popup) {
            // delete the old option buttons
            for (auto& cell : popup->cells) {
                if (cell.widget) {
                    popup->remove_child(cell.widget);
                    delete cell.widget;
                }
            }
            popup->cells.clear();
            build_options();
            if (open) popup->show(); else popup->hide();
        }

        resize();
    }

    void build_options() {
        if (!popup) return;

        float maxOpt = 0.0f;
        for (const auto& o : options)
            maxOpt = std::max(maxOpt, text_width(o, textSize));
        float boxW = maxOpt + HUD_PAD_X * 2.0f + 24.0f;   // 24 = ^ / v
        if (boxW < 100.0f) boxW = 100.0f;

        for (int i = 0; i < (int)options.size(); ++i) {
            Button* b = new Button(0.0f, 0.0f, 1.0f, 1.0f,
                [this, i]() {
                    if (selected) *selected = i;
                    close_popup();
                },
                options[i]);
            b->w = boxW;
            b->prefW = boxW;
            popup->add(b, 0, i, 1, 1, Alignment::STRETCH, Alignment::START);
        }
        popup->layout();
    }


    bool contains(float mx, float my) const {
        if (hit(mx, my)) return true;                       // header
        if (popup && !popup->hidden && popup->hit(mx, my))  // popup panel
            return true;
        return false;
    }

    void resize() {
        labelW = text_width(label, textSize) + HUD_PAD_X * 2.0f;
        if (labelW < 48.0f) labelW = 48.0f;

        float maxOpt = 0.0f;
        for (const auto& o : options)
            maxOpt = std::max(maxOpt, text_width(o, textSize));
        float boxW = maxOpt + HUD_PAD_X * 2.0f + 24.0f;
        if (boxW < 100.0f) boxW = 100.0f;

        w = labelW + boxW + HUD_PAD_X;
        h = textSize + HUD_PAD_Y * 2.0f + 4.0f;
    }

    string currentText() const {
        if (!selected || *selected < 0 || *selected >= (int)options.size())
            return "(none)";
        return options[*selected];
    }

    void draw_self() override {
        modelMatrix = translate(x, (float)hh - h - y, 0.0f) * scale(w, h, 1.0f);
        draw_solid_hud_quad(color);
        draw_string(label, x + HUD_PAD_X, y + HUD_PAD_Y, textSize);

        float boxX = x + labelW;
        float boxW = w - labelW - HUD_PAD_X;
        modelMatrix = translate(boxX, (float)hh - (h - 8.0f) - (y + 4.0f), 0.0f)
                     * scale(boxW, h - 8.0f, 1.0f);
        draw_solid_hud_quad(enabled ? selectedColor: disabledColor);
        draw_string(currentText(), boxX + 6.0f, y + 6.0f, textSize, enabled ? ALT_TEXT_COLOR: DISABLED_TEXT_COLOR);
        draw_string(open ? "^" : "v", boxX + boxW - 22.0f, y + 6.0f, textSize, enabled? ALT_TEXT_COLOR: DISABLED_TEXT_COLOR);
    }

    void open_popup() {
        if (!popup) {
            popup = new Panel(x + labelW, y + h);
            popup->set_priority(1);
            build_options();
        }

        float boxW = w - labelW - HUD_PAD_X;
        popup->x = x + labelW;
        popup->y = y + h;
        popup->w = boxW;
        popup->stretched = true;
        popup->layout();
        popup->show();
        open = true;
    }



    void close_popup() {
        if (popup) popup->hide();
        open = false;
    }

    void on_click(float mx, float my) override {
        if (!enabled) return;
        if (!open) {
            open = true;
            open_popup();
        } else {
            close_popup();
        }
    }

};

#endif //CS370_FA26_DROPDOWN_H
