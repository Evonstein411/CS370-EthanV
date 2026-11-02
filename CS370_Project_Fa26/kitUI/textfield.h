#ifndef CS370_FA26_TEXTFIELD_H
#define CS370_FA26_TEXTFIELD_H

struct TextFieldBase;
template<typename T> struct TextField;

//base for text field needed for callback logic
struct TextFieldBase : Widget {
    bool focused = false;
    bool editable = true;


    TextFieldBase(float x, float y, float w, float h)
        : Widget(x, y, w, h) {}

    void setEditable(bool status) { editable = status; }

    void on_click(float mx, float my) override {
        if (editable) focused = true;
    }
    virtual void commit() = 0;
    virtual const string& getText() const = 0;
    virtual void setText(string n_string) = 0;

};

//Text field for user input
template<typename T>
struct TextField : TextFieldBase {
    vec4 color = HUD_GRAY;
    vec4 fieldColor = HUD_FIELD;
    vec4 textColor = ALT_TEXT_COLOR;
    vec4 disabledColor = HUD_DISABLED;
    string label;
    string text;
    int maxLen;
    float textSize = HUD_FONT;
    float labelW;
    bool isNum = false;
    T* value = nullptr;// pointer to the actual value

    //constructor
    TextField(float x, float y, float w, float h, const string& label, int maxLen = 16, bool isNum = false, T* value = nullptr)
        :TextFieldBase(x, y, w, h), label(label), maxLen(maxLen), isNum(isNum), value(value) {
        resize();
    }

    void commit() override;

    void setEditable(bool status) {
        editable = status;
    }


    const string& getText() const override {
        return text;
    }

    bool valid_number(const string& s) const {
        if (s.empty()) return true;
        size_t i = 0;
        if (s[0] == '-') {
            if (s.size() == 1) return true;
            i = 1;
        }
        bool dot = false;
        bool digit = false;
        for (; i < s.size(); i++) {
            if (s[i] == '.') {
                if (dot) return false;
                dot = true;
                continue;
            }
            if (s[i] < '0' || s[i] > '9') return false;
            digit = true;
        }
        return digit || dot; // ".", "-", "-." while still typing
    }


    void resize() {
        labelW = text_width(label, textSize) + HUD_PAD_X * 2.0f;
        if (labelW < 36.0f) labelW = 36.0f;

        float inner = text_width(text, textSize) + 12.0f;
        float minInner = text_width(" ", textSize) + 12.0f;
        if (inner < minInner) inner = minInner;
        w = labelW + inner + HUD_PAD_X;
        h = textSize + HUD_PAD_Y * 2.0f + 4.0f;
    }

    void setText(string n_text) override{
        //prevent non numbers if needed
        if (isNum && !valid_number(n_text)) return;
        //prevent input from exceeding maxLen
        if (n_text.length() > maxLen){
            n_text.resize(maxLen);
        }
        text = n_text;
        resize();
    }

    void drawSelf() override {
        model_matrix = mat4().identity();
        mat4 scale_matrix, trans_matrix = mat4().identity();
        model_matrix = translate(x, (float)hh - h - y, 0.0f) * scale(w, h, 1.0f);
        draw_solid_hud_quad(color);
        draw_string(label, x + 8.0f, y + 8.0f, 32);

        float boxX = x + labelW;
        float boxY = y + 6.0f;
        float boxH = h - 12.0f;
        float boxW = w - labelW - HUD_PAD_X;
        if (boxW < HUD_PAD_X) boxW = HUD_PAD_X;
        if (boxH < HUD_PAD_Y) boxH = HUD_PAD_Y;

        trans_matrix = translate(boxX, (float)hh - boxH - boxY, 0.0f);
        scale_matrix = scale(boxW, boxH, 1.0f);
        model_matrix =  trans_matrix * scale_matrix;
        draw_solid_hud_quad(editable ? fieldColor : disabledColor);
        draw_string(text, boxX + 6.0f, boxY + 2.0f , textSize, textColor);

        if (focused){
            float textW = text_width(text, textSize);
            float caretX = x + labelW + 4.0f + textW + 1.0f;
            float caretY = boxY + 10.0f;          // same top as the text
            drawCaret(caretX, caretY);
        }
    }

    void on_char(unsigned int charCode) override {
        if (!editable || !focused) return;
        if ((int)text.size() >= maxLen) return; //do nothing if size is at max len
        if (charCode < 32 || charCode > 126) return; //prevent invalid characters
        char c = (char)charCode;

        if (isNum) {
            string next = text;
            next.push_back(c);
            if (!valid_number(next)) return;
        }
        text.push_back(c);
        resize();
    }

    void on_key(int key, int action) override {
        if (!focused || !editable) return;
        if (action != GLFW_PRESS && action != GLFW_REPEAT) return;
        if (key == GLFW_KEY_BACKSPACE && !text.empty()) {
            text.pop_back();
            resize();
        }

    }

    void drawCaret(float caretX, float caretY) {
        // 1 second full cycle → 0.5s on, 0.5s off
        if (fmod((float)glfwGetTime(), 1.0f) > 0.5f)
            return;

        float cw = 2.0f;
        float ch = textSize - 8.0f;

        mat4 trans_matrix = translate(caretX, (float)hh - ch - caretY, 0.0f);
        mat4 scale_matrix = scale(cw, ch, 1.0f);
        model_matrix = trans_matrix * scale_matrix;
        draw_solid_hud_quad(textColor);
    }



};//end Text Field

//Text Field templates
template<>
inline void TextField<float>::commit() {
    if (!value) return;
    char* end = nullptr;
    float n = strtof(text.c_str(), &end);
    if (end != text.c_str())
        *value = n;
}

template<>
inline void TextField<int>::commit() {
    if (!value) return;
    char* end = nullptr;
    long n = strtol(text.c_str(), &end, 10);
    if (end != text.c_str())
        *value = (int)n;
}

template<>
inline void TextField<string>::commit() {
    if (value)
        *value = text;
}

//generic fallback
template<typename T>
inline void TextField<T>::commit() {}




#endif //CS370_FA26_TEXTFIELD_H
