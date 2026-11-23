#ifndef CS370_FA26_TEXTFIELD_H
#define CS370_FA26_TEXTFIELD_H

#include "widget.h"
#include "rendercontext.h"
#include <string>
#include "../common/GLFWutils.h"

struct TextFieldBase : Widget {
    bool focused = false;
    bool editable = true;

    TextFieldBase(float w, float h);

    virtual void set_editable(bool status);
    void on_click(float mx, float my) override;

    virtual void commit() = 0;
    virtual const std::string& get_text() const = 0;
    virtual void set_text(std::string newString) = 0;
};

template<typename T>
struct TextField : TextFieldBase {
    vmath::vec4 color = HUD_DROPFIELD;
    vmath::vec4 fieldColor = HUD_FIELD;
    vmath::vec4 textColor = ALT_TEXT_COLOR;
    vmath::vec4 disabledColor = HUD_DISABLED;

    std::string label;
    std::string text;
    int maxLen;
    float textSize = HUD_FONT;
    float labelW = 0.0f;
    bool isNum = false;
    T* value = nullptr;

    TextField(float w, float h, std::string label,
              int maxLen = 16, bool isNum = false, T* value = nullptr);


    void refresh();
    void commit() override;
    void set_editable(bool status) override;
    const std::string& get_text() const override;
    void set_text(std::string newText) override;

    void resize(const RenderContext& ctx);
    void draw_self(const RenderContext& ctx) override;
    void on_char(unsigned int charCode) override;
    void on_key(int key, int action) override;

private:
    static bool valid_number(const std::string& s);
    void draw_caret(const RenderContext& ctx, float caretX, float caretY) const;
};

// template implementations
template<typename T>
TextField<T>::TextField(float w, float h, std::string label,
                        int maxLen, bool isNum, T* value)
    : TextFieldBase(w, h), label(std::move(label)),
      maxLen(maxLen), isNum(isNum), value(value) {}


template<typename T>
void TextField<T>::set_editable(bool status) {
    editable = status;
}

template<typename T>
const std::string& TextField<T>::get_text() const {
    return text;
}

template<typename T>
bool TextField<T>::valid_number(const std::string& s) {
    if (s.empty()) return true;
    size_t i = 0;
    if (s[0] == '-') {
        if (s.size() == 1) return true;
        i = 1;
    }
    bool dot = false, digit = false;
    for (; i < s.size(); ++i) {
        if (s[i] == '.') {
            if (dot) return false;
            dot = true;
            continue;
        }
        if (s[i] < '0' || s[i] > '9') return false;
        digit = true;
    }
    return digit || dot;
}

template<typename T>
void TextField<T>::resize(const RenderContext& ctx) {
    labelW = ctx.text_width(label, textSize) + HUD_PAD_X * 2.0f;
    if (labelW < 36.0f) labelW = 36.0f;

    float inner = ctx.text_width(text, textSize) + 12.0f;
    float minInner = ctx.text_width(" ", textSize) + 12.0f;
    if (inner < minInner) inner = minInner;

    w = labelW + inner + HUD_PAD_X;
    h = textSize + HUD_PAD_Y * 2.0f + 4.0f;
    prefW = w;
    prefH = h;
}

template<typename T>
void TextField<T>::set_text(std::string newText) {
    if (isNum && !valid_number(newText)) return;
    if (newText.length() > static_cast<size_t>(maxLen))
        newText.resize(maxLen);
    text = std::move(newText);
    // resize deferred to next draw (needs context)
}

template<typename T>
void TextField<T>::draw_self(const RenderContext& ctx) {
    resize(ctx);

    // background
    vmath::mat4 model = vmath::translate(x, y, 0.0f) * vmath::scale(w, h, 1.0f);
    ctx.draw_solid_quad(color, model);
    ctx.draw_string(label, x + 8.0f, y + 8.0f, textSize);

    // input box
    const float boxX = x + labelW;
    const float boxY = y + 6.0f;
    float boxH = h - 12.0f;
    float boxW = w - labelW - HUD_PAD_X;
    if (boxW < HUD_PAD_X) boxW = HUD_PAD_X;
    if (boxH < HUD_PAD_Y) boxH = HUD_PAD_Y;

    vmath::mat4 boxModel = vmath::translate(boxX, boxY, 0.0f) * vmath::scale(boxW, boxH, 1.0f);
    ctx.draw_solid_quad(editable ? fieldColor : disabledColor, boxModel);

    vmath::vec4 tc = editable ? textColor : DISABLED_TEXT_COLOR;
    ctx.draw_string(text, boxX + 6.0f, boxY + 2.0f, textSize, tc);

    if (focused) {
        float textW = ctx.text_width(text, textSize);
        float caretX = boxX + 6.0f + textW + 1.0f;
        float caretY = boxY + 2.0f;
        draw_caret(ctx, caretX, caretY);
    }
}

template<typename T>
void TextField<T>::draw_caret(const RenderContext& ctx, float caretX, float caretY) const {
    if (fmod(static_cast<float>(glfwGetTime()), 1.0f) > 0.5f) return;

    constexpr float cw = 2.0f;
    float ch = textSize - 8.0f;
    vmath::mat4 model = vmath::translate(caretX, caretY, 0.0f) * vmath::scale(cw, ch, 1.0f);
    ctx.draw_solid_quad(textColor, model);
}

template<typename T>
void TextField<T>::on_char(unsigned int charCode) {
    if (!editable || !focused) return;
    if (static_cast<int>(text.size()) >= maxLen) return;
    if (charCode < 32 || charCode > 126) return;

    char c = static_cast<char>(charCode);
    if (isNum) {
        std::string next = text;
        next.push_back(c);
        if (!valid_number(next)) return;
    }
    text.push_back(c);
}

template<typename T>
void TextField<T>::on_key(int key, int action) {
    if (!focused || !editable) return;
    if (action != GLFW_PRESS && action != GLFW_REPEAT) return;
    if (key == GLFW_KEY_BACKSPACE && !text.empty())
        text.pop_back();
}

// specializations
//FLOAT
template<>
inline void TextField<float>::refresh() {
    if (!value) return;
    text = std::to_string(*value);
}

template<>
inline void TextField<float>::commit() {
    if (!value) return;
    char* end = nullptr;
    float n = strtof(text.c_str(), &end);
    if (end != text.c_str()) *value = n;
}

//INT
template<>
inline void TextField<int>::refresh() {
    if (!value) return;
    text = std::to_string(*value);
}

template<>
inline void TextField<int>::commit() {
    if (!value) return;
    char* end = nullptr;
    long n = strtol(text.c_str(), &end, 10);
    if (end != text.c_str()) *value = static_cast<int>(n);
}

//STRING
template<>
inline void TextField<std::string>::refresh() {
    if (!value) return;
    text = *value;
}

template<>
inline void TextField<std::string>::commit() {
    if (value) *value = text;
}

//FALLBACK
template<typename T>
inline void TextField<T>::refresh() {}

template<typename T>
inline void TextField<T>::commit() {}

#endif