#include <string>

//structs
struct Widget;
struct Button;
struct TextField;
struct Label;

//list of widgets
extern vector<Widget*> widgets;

//HUD util function defs
void draw_string(const string& text, float x, float y, float size);
float text_width(const string& text, float size);

//HUD globals
const float HUD_FONT = 32.0f;
const float HUD_PAD_X = 8.0f;
const float HUD_PAD_Y = 8.0f;

//parent HUD element struct
struct Widget {
    float x, y, w, h;
    bool hovered = false;

    Widget(float x, float y, float w, float h)
        : x(x), y(y), w(w), h(h) {
        widgets.push_back(this);
    }

    virtual ~Widget() {}
    virtual void draw() = 0; // each child supplies its own draw
    virtual void on_char(unsigned int) {}
    virtual void on_key(int key, int action) {}
    virtual void on_click() {}
    virtual void on_drag(float mx, float my) {}

    bool hit(float mx, float my) const {
        float bottom = (float)hh - h - y;
        return mx >= x && mx <= x + w && my >= bottom && my <= bottom + h;
    }
}; //end Widget

//Button widget for interactivity
struct Button : Widget {
    string label;
    float textSize = HUD_FONT;
    void (*onPress)();

    //constructor (no label specified)
    Button(float x, float y, float w, float h, void (*onPress)(), const string& label = "" )
    : Widget(x, y, w, h), onPress(onPress), label(label) {
        fit();
    }



    void fit() {
        w = text_width(label, textSize) + HUD_PAD_X * 2.0f;
        if (w < 48.0f) w = 48.0f;
        h = textSize + HUD_PAD_Y * 2.0f + 4.0f;
    }

    void draw() override {
        model_matrix = mat4().identity();
        mat4 scale_matrix, trans_matrix = mat4().identity();

        GLuint color = hovered ? HUDHighlighted : HUDGray;

        scale_matrix = scale(w, h, 1.0f);
        trans_matrix = translate(x, (float)hh - h - y, 0.0f);//move to top left for more intuitive positioning increase in y goes down
        model_matrix = trans_matrix * scale_matrix;
        draw_color_object(HUDQuad, color);

        draw_string(label, x + HUD_PAD_X, y + HUD_PAD_Y, textSize);
    }

    void on_click() override {
        if (onPress) onPress();
    }

};//end Button

//Text field for user input
struct TextField : Widget {
    string label;
    string text;
    int maxLen;
    float textSize = HUD_FONT;
    float labelW;
    bool focused;
    bool isNum;
    float* value;
    //constructor
    TextField(float x, float y, float w, float h, const string& label, int maxLen = 16, bool isNum = false, float* value = NULL)
        :Widget(x, y, w, h), label(label), maxLen(maxLen), isNum(isNum), value(value), focused(false) {
        resize();
    }

    void commit() {
        if (!value) return;
        char* end = NULL;
        float n = strtof(text.c_str(), &end);
        if (end == text.c_str()) return; // blank or just "-"
        *value = n;
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

    void setText(string n_text) {
        //prevent non numbers if needed
        if (isNum && !valid_number(n_text)) return;
        //prevent input from exceeding maxLen
        if (n_text.length() > maxLen){
            n_text.resize(maxLen);
        }
        text = n_text;
        resize();
    }

    void draw() override {
        model_matrix = mat4().identity();
        mat4 scale_matrix, trans_matrix = mat4().identity();
        GLuint outer = focused ? HUDHighlighted : HUDGray;
        model_matrix = translate(x, (float)hh - h - y, 0.0f) * scale(w, h, 1.0f);
        draw_color_object(HUDQuad, outer);
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
        draw_color_object(HUDQuad, HUDTextField);
        draw_string(text, boxX + 6.0f, boxY + 2.0f , textSize);
    }

    void on_click() override {
        focused = true;
    }

    void on_char(unsigned int charCode) override {
        if (!focused) return; //do nothing if not focused
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
        if (!focused) return;
        if (action != GLFW_PRESS && action != GLFW_REPEAT) return;
        if (key == GLFW_KEY_BACKSPACE && !text.empty()) {
            text.pop_back();
            resize();
        }

    }

};//end Text Field

//Label for labeling UI sections
struct Label : Widget {
    string label;
    float textSize = HUD_FONT;

    Label(float x, float y, float w, float h, const string& label)
    : Widget(x, y, w, h), label(label) {
        fit();
    }

    void fit() {
        w = text_width(label, textSize) + HUD_PAD_X * 2.0f;
        if (w < 48.0f) w = 48.0f;
        h = textSize + HUD_PAD_Y * 2.0f + 4.0f;
    }

    void draw() override {
        model_matrix = mat4().identity();
        mat4 scale_matrix, trans_matrix = mat4().identity();

        scale_matrix = scale(w, h, 1.0f);
        trans_matrix = translate(x, (float)hh - h - y, 0.0f);
        model_matrix = trans_matrix * scale_matrix;
        draw_color_object(HUDQuad, HUDGray);

        draw_string(label, x + 8.0f, y + 6.0f, textSize);
    }

};//end Label


//util function for drawing text
void draw_string(const string& text, float x, float y, float size) {
    float scale = size / 32.0f;   // bake height is 32

    glUseProgram(texture_program);
    glUniformMatrix4fv(texture_proj_mat_loc, 1, GL_FALSE, proj_matrix);
    glUniformMatrix4fv(texture_camera_mat_loc, 1, GL_FALSE, camera_matrix);
    glUniform2f(texture_uv_scale_loc, 1.0f, 1.0f);
    glUniform2f(texture_uv_offset_loc, 0.0f, 0.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TextureIDs[Font]);

    float penX = x;
    float baseline = y + size;    // y is pixels down from the top

    for (size_t i = 0; i < text.size(); i++) {
        if (text[i] < 32 || text[i] > 126) continue;

        float startX = penX;
        float startBase = baseline;
        stbtt_aligned_quad q;
        stbtt_GetBakedQuad(glyphs, 512, 512, text[i] - 32, &penX, &baseline, &q, 1);

        float gx0 = startX + (q.x0 - startX) * scale;
        float gx1 = startX + (q.x1 - startX) * scale;
        float gy0 = startBase + (q.y0 - startBase) * scale;
        float gy1 = startBase + (q.y1 - startBase) * scale;
        penX = startX + (penX - startX) * scale;
        baseline = startBase;

        float top = (float)hh - gy0;
        float bot = (float)hh - gy1;
        vec4 pos[6] = {
            vec4(gx0, top, 0, 1), vec4(gx1, top, 0, 1), vec4(gx1, bot, 0, 1),
            vec4(gx0, top, 0, 1), vec4(gx1, bot, 0, 1), vec4(gx0, bot, 0, 1)
        };
        vec2 uv[6] = {
            vec2(q.s0, q.t0), vec2(q.s1, q.t0), vec2(q.s1, q.t1),
            vec2(q.s0, q.t0), vec2(q.s1, q.t1), vec2(q.s0, q.t1)
        };

        glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[HUDTextQuad][PosBuffer]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), pos);
        glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[HUDTextQuad][TexBuffer]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(uv), uv);

        model_matrix = mat4().identity();
        draw_tex_object(HUDTextQuad, Font);
    }
}

//util function for calculating text width
float text_width(const string& text, float size) {
    float scale = size / 32.0f;
    float width = 0.0f;
    for (size_t i = 0; i < text.size(); i++) {
        if (text[i] < 32 || text[i] > 126) continue;
        width += glyphs[text[i] - 32].xadvance * scale;
    }
    return width;
}
