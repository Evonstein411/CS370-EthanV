#include <string>
#include <functional>
#include <cstdlib>


//structs
struct Widget;
struct Button;
struct TextFieldBase;
template<typename T> struct TextField;
struct Label;
struct Panel;
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
    bool hidden = false;
    int priority = 0;

    vector<Widget*> children; //list of all current children
    Widget* parent = nullptr; //pointer to parent if have one

    Widget(float x, float y, float w, float h)
        : x(x), y(y), w(w), h(h) {
        widgets.push_back(this);
    }

    virtual ~Widget() {
        if (parent) parent->removeChild(this); //if im a child remove myself from parent
        for (Widget* c : children) c->parent = nullptr; //clear self from parent of all children
        children.clear(); //clear children list
    }

    virtual void drawSelf() = 0; //defined per widget
    virtual void on_char(unsigned int) {}
    virtual void on_key(int key, int action) {}
    virtual void on_click() {}
    virtual void on_drag(float mx, float my) {}
    virtual void on_hover(float mx, float my){}

    virtual bool hit(float mx, float my) const {
        if (hidden || isEffectivelyHidden()) return false;
        float bottom = (float)hh - h - y;
        return mx >= x && mx <= x + w && my >= bottom && my <= bottom + h;
    }

    void draw() {
        if(hidden) {
            return;
        }
        drawSelf();
        for(Widget* child: children) {
            child->draw();
        }

    }

    void hide() {
        hidden = true;
    }

    void show() {
        hidden = false;
    }

    void setHidden(bool new_hidden) {
        hidden = new_hidden;
    }

    Widget* getParent() {
        return parent;
    }

    void addChild(Widget* child) {
        if(child == this) return; //cant child self

        //remove from any prior parent
        if (child->isChild()) {
            child->getParent()->removeChild(child);
        }

        child->parent = this;
        children.push_back(child);
    }

    void removeChild(Widget* child) {
        if(!child) return; //do nothing if child doesn't exist
        auto searched_child = std::find(children.begin(), children.end(), child);
        if (searched_child == children.end()) return; //failed to find child do nothing
        children.erase(searched_child); //found child remove from children list
        if (child->getParent() == this) { //remove self from parent of prior child
            child->parent = nullptr;
        }
    }


    //only remove and add child directly write to parent to prevent desync of child and parent status
    void setParent(Widget* new_parent) {
        if(new_parent == parent) return; //do nothing if already parent
        if(new_parent) {
            new_parent->addChild(this);   //add to new parent
        }
        else if(parent) {
            parent->removeChild(this);
        }
    }

    bool isChild() const {
        return parent != nullptr;
    }

    bool isHidden() {
        return hidden;
    }

    bool isEffectivelyHidden() const {
        for (const Widget* p = parent; p != nullptr; p = p->parent) {
            if (p->hidden) return true;
        }
        return false;
    }




}; //end Widget




// ------------------------------------------------------------------- Widget Child Structs ----------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------

//Panel widget for widget organization
struct Panel : Widget {

    //cell holds widget and information on placement in panel
    struct Cell {
        Widget* widget = NULL;
        int col = 0;
        int row = 0;
        int colSpan = 1;
        int rowSpan = 1;
    };

    vector<Cell> cells;
    float padX = HUD_PAD_X;
    float padY = HUD_PAD_Y;
    float gapX = 6.0f;
    float gapY = 12.0f;
    bool drawBackground = true;


    Panel(float x, float y)
    : Widget(x, y, 0.0f, 0.0f) {

    }

    //destructor handle deleting cells and removing child status from widgets
    ~Panel() {
        for (size_t i = 0; i < cells.size(); i++)
            removeChild(cells[i].widget);
        cells.clear();
    }


    //add widget to layout
    void add(Widget* w, int col, int row, int colSpan = 1, int rowSpan = 1) {
        Cell c;
        c.widget = w;
        c.col = col;
        c.row = row;
        c.colSpan = colSpan;
        c.rowSpan = rowSpan;
        cells.push_back(c);
        addChild(w);
    }
    
    void layout() {
        if (cells.empty()) {
            w = padX * 2.0f;
            h = padY * 2.0f;
            return;
        }

        if (hidden) {
            w = 0.0f;
            h = 0.0f;
            return;
        }

        int maxCol = 0, maxRow = 0;
        for (size_t i = 0; i < cells.size(); i++) {
            int c = cells[i].col + cells[i].colSpan - 1;
            int r = cells[i].row + cells[i].rowSpan - 1;
            if (c > maxCol) maxCol = c;
            if (r > maxRow) maxRow = r;
        }
        int nCols = maxCol + 1;
        int nRows = maxRow + 1;

        // Column widths and row heights driven by content
        vector<float> colW(nCols, 0.0f);
        vector<float> rowH(nRows, 0.0f);

        for (size_t i = 0; i < cells.size(); i++) {
            Cell& c = cells[i];
            if (c.widget->hidden) continue;
            if (c.colSpan == 1 && c.widget->w > colW[c.col])
                colW[c.col] = c.widget->w;
            if (c.rowSpan == 1 && c.widget->h > rowH[c.row])
                rowH[c.row] = c.widget->h;
        }


        // Prefix sums for easy positioning
        vector<float> colX(nCols + 1, 0.0f);
        vector<float> rowY(nRows + 1, 0.0f);
        for (int i = 0; i < nCols; i++)
            colX[i + 1] = colX[i] + colW[i] + (i < nCols - 1 ? gapX : 0.0f);
        for (int i = 0; i < nRows; i++)
            rowY[i + 1] = rowY[i] + rowH[i] + (i < nRows - 1 ? gapY : 0.0f);

        // Position children relative to the panel
        for (size_t i = 0; i < cells.size(); i++) {
            Cell& c = cells[i];
            float cx = colX[c.col];
            float cy = rowY[c.row];
            float cw = colX[c.col + c.colSpan] - colX[c.col]
                     - (c.colSpan > 1 ? gapX * (c.colSpan - 1) : 0.0f);
            float ch = rowY[c.row + c.rowSpan] - rowY[c.row]
                     - (c.rowSpan > 1 ? gapY * (c.rowSpan - 1) : 0.0f);

            float px = x + padX + cx;
            float py = y + padY + cy;

            // Center only if the widget spans more than one cell
            if (c.colSpan > 1 || c.rowSpan > 1) {
                float offsetX = (cw - c.widget->w) * 0.5f;
                float offsetY = (ch - c.widget->h) * 0.5f;
                if (offsetX < 0.0f) offsetX = 0.0f;
                if (offsetY < 0.0f) offsetY = 0.0f;
                px += offsetX;
                py += offsetY;
            }

            c.widget->x = px;
            c.widget->y = py;
        }

        w = padX * 2.0f + colX[nCols];
        h = padY * 2.0f + rowY[nRows];

    }//end layout()

    void drawSelf() override {
        //update layout before drawing
        layout();
        if (drawBackground) {
            mat4 trans_matrix = translate(x, (float)hh - h - y, 0.0f);
            mat4 scale_matrix = scale(w, h, 1.0f);
            model_matrix = trans_matrix * scale_matrix;
            draw_color_object(HUDQuad, HUDPanel);
        }

    }//end draw()



};//end Panel



//Button widget for interactivity
struct Button : Widget {
    string label;
    float textSize = HUD_FONT;
    std::function<void()> onPress;

    //constructor (no label specified)
    Button(float x, float y, float w, float h, std::function<void()> onPress, const string& label = "" )
    : Widget(x, y, w, h), onPress(onPress), label(label) {
        fit();
    }



    void fit() {
        w = text_width(label, textSize) + HUD_PAD_X * 2.0f;
        if (w < 48.0f) w = 48.0f;
        h = textSize + HUD_PAD_Y * 2.0f + 4.0f;
    }

    void drawSelf() override {
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


//base for text field needed for callback logic
struct TextFieldBase : Widget {
    bool focused = false;
    bool editable = true;

    TextFieldBase(float x, float y, float w, float h)
        : Widget(x, y, w, h) {}

    void setEditable(bool status) { editable = status; }

    void on_click() override {
        if (editable) focused = true;
    }
    virtual void commit() = 0;
    virtual const string& getText() const = 0;
    virtual void setText(string n_string) = 0;

};

//Text field for user input
template<typename T>
struct TextField : TextFieldBase {
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

    void drawSelf() override {
        model_matrix = mat4().identity();
        mat4 scale_matrix, trans_matrix = mat4().identity();

        scale_matrix = scale(w, h, 1.0f);
        trans_matrix = translate(x, (float)hh - h - y, 0.0f);
        model_matrix = trans_matrix * scale_matrix;
        draw_color_object(HUDQuad, HUDGray);

        draw_string(label, x + 8.0f, y + 6.0f, textSize);
    }

};//end Label

struct Section : Widget {
    string label;
    bool collapsed = false;
    Panel* content;
    float headerH = 0.0f;

    Section(float x, float y, const string& label)
        : Widget(x, y, 0, 0), label(label) {
        headerH = HUD_FONT + HUD_PAD_Y * 2.0f;
        content = new Panel(x, y);
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
        headerH = HUD_FONT + HUD_PAD_Y * 2.0f;

        if (collapsed || content->cells.empty()) {
            float tw = text_width(label, HUD_FONT) + HUD_PAD_X * 3.0f + 20.0f;
            w = std::max(tw, 48.0f);
            h = headerH;
            return;
        }

        content->x = x;
        content->y = y + headerH;
        content->layout();

        w = std::max(content->w, text_width(label, HUD_FONT) + HUD_PAD_X * 3.0f + 20.0f);
        h = headerH + content->h;

    }

    void drawSelf() override {
        layout();

        // header bar
        mat4 trans_matrix = translate(x, (float)hh - headerH - y, 0.0f);
        mat4 scale_matrix   = scale(w, headerH, 1.0f);
        model_matrix = trans_matrix * scale_matrix;
        draw_color_object(HUDQuad, hovered ? HUDHighlighted : HUDGray);

        string arrow = collapsed ? ">" : "v";
        draw_string(arrow, x + HUD_PAD_X, y + HUD_PAD_Y, HUD_FONT);
        draw_string(label, x + HUD_PAD_X + 20.0f, y + HUD_PAD_Y, HUD_FONT);


    }

    void on_click() override {
        // mouse callback already confirmed we were the top-most hit
        collapsed = !collapsed;
        content->setHidden(collapsed);
        layout();
    }


};//end Section



struct Dropdown : Widget {
    string label;
    vector<string> options;
    int* selected = nullptr;   // binds to an int (index into options)
    bool open = false;
    bool closing = false;
    float textSize = HUD_FONT;
    float labelW = 0.0f;

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

    void drawSelf() override {
        GLuint outer = hovered || open ? HUDHighlighted : HUDGray;
        model_matrix = translate(x, (float)hh - h - y, 0.0f) * scale(w, h, 1.0f);
        draw_color_object(HUDQuad, outer);
        draw_string(label, x + HUD_PAD_X, y + HUD_PAD_Y, textSize);

        float boxX = x + labelW;
        float boxW = w - labelW - HUD_PAD_X;
        model_matrix = translate(boxX, (float)hh - (h - 8.0f) - (y + 4.0f), 0.0f)
                     * scale(boxW, h - 8.0f, 1.0f);
        draw_color_object(HUDQuad, HUDTextField);
        draw_string(currentText(), boxX + 6.0f, y + 6.0f, textSize);
        draw_string(open ? "^" : "v", boxX + boxW - 22.0f, y + 6.0f, textSize);
    }

    void openPopup() {
        if (!popup) {
            popup = new Panel(x + labelW, y + h);

            for (int i = 0; i < (int)options.size(); ++i) {
                Button* b = new Button(0.0f, 0.0f, 1.0f, 1.0f,
                    [this, i]() {
                        if (selected) *selected = i;
                        closePopup();          // only hides
                    },
                    options[i]);
                popup->add(b, 0, i);
            }
            popup->layout();
        }

        popup->x = x + labelW;
        popup->y = y + h;
        popup->layout();
        popup->show();
        open = true;
    }



    void closePopup() {
        if (popup) popup->hide();
        open = false;
    }

    void on_click() override {
        if (!open) {
            open = true;
            openPopup();
        } else {
            closePopup();
        }
    }

};

















// -------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------- UTILITY FUNCTONS ------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------------------------


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
