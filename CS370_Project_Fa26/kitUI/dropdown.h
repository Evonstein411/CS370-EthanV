#ifndef CS370_FA26_DROPDOWN_H
#define CS370_FA26_DROPDOWN_H

#include "widget.h"
#include "rendercontext.h"
#include "panel.h"
#include "button.h"
#include <string>
#include <vector>
#include <functional>

struct Dropdown : Widget {
    std::string label;
    std::vector<std::string> options;
    int* selected = nullptr;
    bool open = false;
    bool enabled = true;
    float textSize = HUD_FONT;
    float labelW = 0.0f;
    vmath::vec4 color = HUD_DROPFIELD;
    vmath::vec4 highlightedColor = HUD_HIGHLIGHTED;
    vmath::vec4 selectedColor = HUD_FIELD;
    vmath::vec4 disabledColor = HUD_DISABLED;

    Panel* popup = nullptr;

    Dropdown(float w, float h, std::string label,
             const std::vector<std::string>& options, int* selected);
    Dropdown(float x, float y, float w, float h, std::string label,
             const std::vector<std::string>& options, int* selected);

    template<typename E>
    Dropdown(float x, float y, float w, float h,
             const std::string& label,
             const std::vector<std::string>& options,
             E& selectedEnum)
        : Dropdown(x, y, w, h, label, options, reinterpret_cast<int*>(&selectedEnum)) {}

    ~Dropdown() override;

    void set_enabled(bool on);
    void set_options(const std::vector<std::string>& newOptions);

    bool contains(float mx, float my) const;
    void resize(const RenderContext& ctx);
    std::string currentText() const;

    void draw_self(const RenderContext& ctx) override;
    void on_click(float mx, float my) override;

    void open_popup(const RenderContext& ctx);
    void close_popup();

private:
    void build_options(const RenderContext& ctx);
};

#endif