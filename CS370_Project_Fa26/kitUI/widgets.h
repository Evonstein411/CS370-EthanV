#ifndef KITUI_WIDGETS_H
#define KITUI_WIDGETS_H

#include <string>
#include <functional>
#include <cstdlib>
#include "../drawObjects.h"


//structs
struct Widget;
struct Button;
struct TextFieldBase;
template<typename T> struct TextField;
struct Label;
struct Panel;

//list of widgets
inline vector<Widget*> widgets;
inline vector<Widget*> trash;
inline bool handlingEvent = false;
inline int eventDepth = 0;
//HUD globals
const vec4 HUD_GRAY = vec4(0.4f, 0.4f, 0.4f, 0.85f);
const vec4 HUD_PANEL = vec4(0.3f, 0.3f, 0.3f, 0.85);
const vec4 HUD_HIGHLIGHTED = vec4(0.8f, 0.8f, 0.8f, 0.85f);
const vec4 HUD_FIELD = vec4(0.9f, 0.9f, 0.9f, 0.85f);
const vec4 HUD_DISABLED = vec4(0.6f, 0.6f, 0.6f, 0.85f);
const float HUD_FONT = 32.0f;
const float HUD_PAD_X = 8.0f;
const float HUD_PAD_Y = 8.0f;
const vec4 TEXT_COLOR = vec4(1.0f, 1.0f, 1.0f, 1.0f);
const vec4 ALT_TEXT_COLOR = vec4(0.2f, 0.2f, 0.2f, 1.0f);

enum class Alignment {START, CENTER, END, STRETCH};


//HUD util function defs
void draw_string(const string& text, float x, float y, float size,
                 vec4 color = TEXT_COLOR);
float text_width(const string& text, float size);




//parent HUD element struct
struct Widget {
    float x, y, w, h;
    float prefW = 0.0f;
    float prefH = 0.0f;
    bool hovered = false;
    bool hidden = false;
    bool grabbed = false;
    bool pendingDelete = false;
    int priority = 0; //higher priority value drawn first

    std::function<void(float, float)> onDragCallback;
    std::function<void()> onReleaseCallback;

    vector<Widget*> children; //list of all current children
    Widget* parent = nullptr; //pointer to parent if have one

    Widget(float x, float y, float w, float h)
        : x(x), y(y), w(w), h(h) {
        widgets.push_back(this);
    }

    virtual ~Widget() {
        auto it = std::find(widgets.begin(), widgets.end(), this);
        if (it != widgets.end())
            widgets.erase(it);

        if (parent) parent->removeChild(this); //if im a child remove myself from parent
        for (Widget* c : children) c->parent = nullptr; //clear self from parent of all children
        children.clear(); //clear children list

    }

    virtual void drawSelf() = 0; //defined per widget
    virtual void drawAfter(){}
    virtual void on_char(unsigned int) {}
    virtual void on_key(int key, int action) {}
    virtual void on_click(float mx, float my) {}
    virtual void on_hover(float mx, float my){}

    virtual bool hit(float mx, float my) const {
        if (hidden || isEffectivelyHidden()) return false;
        float bottom = (float)hh - h - y;
        return mx >= x && mx <= x + w && my >= bottom && my <= bottom + h;
    }

    //function for drawing children
    virtual void drawChildren() {
        for (Widget* c : children) {
            c->draw();
        }
    }

    void markForDelete() {
        if (pendingDelete) return;
        pendingDelete = true;
        hidden = true;
    }

    //if hidden skip else draws self, draws children, and finally draws after for any drawing that needs to be done after children
    void draw() {
        if(hidden) {
            return;
        }
        drawSelf();
        drawChildren();
        drawAfter();

    }

    //interaction

    virtual void on_drag(float mx, float my) {
        if (onDragCallback) {onDragCallback(mx, my);}
    }

    virtual void on_release() {
        grabbed = false;
        if (onReleaseCallback) onReleaseCallback();
    }

    void grab() {
        grabbed = true;
    }


    //parent child relations
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


    //visibility status
    void setPriority(int new_priority) {
        priority = new_priority;
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

    void hide() {
        hidden = true;
    }

    void show() {
        hidden = false;
    }

    void setHidden(bool new_hidden) {
        hidden = new_hidden;
    }


}; //end Widget


//widget deletion utils
inline void destroy_widget(Widget* w) {
    if (!w || w->pendingDelete) return;
    if (handlingEvent) {
        w->markForDelete();
        trash.push_back(w);
    } else {
        delete w;
    }
}

inline void flush_destroyed() {
    if (trash.empty()) return;
    vector<Widget*> batch;
    batch.swap(trash);
    for (Widget* w : batch)
        delete w;                      // destructor already unregisters from widgets
}

//place this object in any callback/event to cleanly handle deletion
struct EventScope {
    EventScope() { eventDepth++; handlingEvent = true; }
    ~EventScope() {
        if (--eventDepth == 0) {
            handlingEvent = false;
            flush_destroyed();
        }
    }
};

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
        Alignment xAlign = Alignment::START;
        Alignment yAlign = Alignment::START;
    };

    vector<Cell> cells;
    float padX = HUD_PAD_X;
    float padY = HUD_PAD_Y;
    float gapX = 6.0f;
    float gapY = 12.0f;
    bool drawBackground = true;
    vec4 color = HUD_PANEL;


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
    void add(Widget* w, int col, int row, int colSpan = 1, int rowSpan = 1,
        Alignment xAlign = Alignment::START,
        Alignment yAlign = Alignment::START) {
        Cell c;
        c.widget = w;
        c.col = col;
        c.row = row;
        c.colSpan = colSpan;
        c.rowSpan = rowSpan;
        c.xAlign = xAlign;
        c.yAlign = yAlign;
        cells.push_back(c);
        addChild(w);
    }

virtual void layout() {
    if (cells.empty()) { w = padX * 2.0f; h = padY * 2.0f; return; }
    if (hidden)        { w = 0.0f;        h = 0.0f;        return; }

    // ----------------------------------------------------------
    // 1. how many columns and rows
    // ----------------------------------------------------------
    int maxCol = 0, maxRow = 0;
    for (size_t i = 0; i < cells.size(); i++) {
        int c = cells[i].col + cells[i].colSpan - 1;
        int r = cells[i].row + cells[i].rowSpan - 1;
        if (c > maxCol) maxCol = c;
        if (r > maxRow) maxRow = r;
    }
    int nCols = maxCol + 1;
    int nRows = maxRow + 1;

    // ----------------------------------------------------------
    // 2. column widths and row heights from preferred size
    //    a cell that spans columns still contributes its height
    // ----------------------------------------------------------
    vector<float> colW(nCols, 0.0f);
    vector<float> rowH(nRows, 0.0f);

    for (size_t i = 0; i < cells.size(); i++) {
        Cell& c = cells[i];
        if (!c.widget || c.widget->hidden) continue;

        float pw = c.widget->prefW > 0.0f ? c.widget->prefW : c.widget->w;
        float ph = c.widget->prefH > 0.0f ? c.widget->prefH : c.widget->h;

        if (c.colSpan == 1 && pw > colW[c.col]) colW[c.col] = pw;
        if (c.rowSpan == 1 && ph > rowH[c.row]) rowH[c.row] = ph;
    }

    // ----------------------------------------------------------
    // 3. turn sizes into positions (gap lives here, not in the size)
    // ----------------------------------------------------------
    vector<float> colX(nCols + 1, 0.0f);
    vector<float> rowY(nRows + 1, 0.0f);
    for (int i = 0; i < nCols; i++)
        colX[i + 1] = colX[i] + colW[i] + ((i < nCols - 1 && colW[i] > 0.0f) ? gapX : 0.0f);
    for (int i = 0; i < nRows; i++)
        rowY[i + 1] = rowY[i] + rowH[i] + ((i < nRows - 1 && rowH[i] > 0.0f) ? gapY : 0.0f);

    // ----------------------------------------------------------
    // 4. each cell's box, before any stretch borrowing
    // ----------------------------------------------------------
    struct Place { float x, y, w, h; };
    vector<Place> place(cells.size());

    for (size_t i = 0; i < cells.size(); i++) {
        Cell& c = cells[i];
        if (!c.widget) { place[i] = {}; continue; }

        float cw = 0.0f, ch = 0.0f;
        for (int k = 0; k < c.colSpan; k++) {
            cw += colW[c.col + k];
            if (k + 1 < c.colSpan) cw += gapX;
        }
        for (int k = 0; k < c.rowSpan; k++) {
            ch += rowH[c.row + k];
            if (k + 1 < c.rowSpan) ch += gapY;
        }
        place[i] = { x + padX + colX[c.col], y + padY + rowY[c.row], cw, ch };
    }

    // ----------------------------------------------------------
    // 5. same row only: a STRETCH cell takes unused space from
    //    the neighbor column. path grows, X stays on the right.
    // ----------------------------------------------------------
    for (int row = 0; row < nRows; row++) {
        for (size_t i = 0; i < cells.size(); i++) {
            Cell& donor = cells[i];
            if (!donor.widget || donor.widget->hidden) continue;
            if (donor.row != row || donor.rowSpan != 1) continue;
            if (donor.xAlign == Alignment::STRETCH) continue;

            float used = donor.widget->prefW > 0.0f ? donor.widget->prefW : donor.widget->w;
            float unused = place[i].w - used;
            if (unused <= 1.0f) continue;

            // column to the left is stretch -> it borrows
            for (size_t j = 0; j < cells.size(); j++) {
                Cell& left = cells[j];
                if (!left.widget || left.row != row || left.rowSpan != 1) continue;
                if (left.xAlign != Alignment::STRETCH) continue;
                if (left.col + left.colSpan != donor.col) continue;

                place[j].w += unused;
                place[i].x += unused;
                place[i].w -= unused;
                unused = 0.0f;
                break;
            }
            if (unused <= 0.0f) continue;

            // otherwise a stretch column to the right borrows
            for (size_t j = 0; j < cells.size(); j++) {
                Cell& right = cells[j];
                if (!right.widget || right.row != row || right.rowSpan != 1) continue;
                if (right.xAlign != Alignment::STRETCH) continue;
                if (right.col != donor.col + donor.colSpan) continue;

                place[j].x -= unused;
                place[j].w += unused;
                place[i].w -= unused;
                break;
            }
        }
    }

    // ----------------------------------------------------------
    // 6. write the box back onto the widget
    // ----------------------------------------------------------
    for (size_t i = 0; i < cells.size(); i++) {
        Cell& c = cells[i];
        if (!c.widget) continue;
        Place& p = place[i];

        if (c.xAlign == Alignment::STRETCH) {
            c.widget->w = p.w;
            c.widget->x = p.x;
        } else if (c.xAlign == Alignment::CENTER) {
            c.widget->x = p.x + (p.w - c.widget->w) * 0.5f;
        } else if (c.xAlign == Alignment::END) {
            c.widget->x = p.x + (p.w - c.widget->w);
        } else {
            c.widget->x = p.x;
        }

        if (c.yAlign == Alignment::STRETCH) {
            c.widget->h = p.h;
            c.widget->y = p.y;
        } else if (c.yAlign == Alignment::CENTER) {
            c.widget->y = p.y + (p.h - c.widget->h) * 0.5f;
        } else if (c.yAlign == Alignment::END) {
            c.widget->y = p.y + (p.h - c.widget->h);
        } else {
            c.widget->y = p.y;
        }
    }

    // ----------------------------------------------------------
    // 7. panel size
    // ----------------------------------------------------------
    w = padX * 2.0f + colX[nCols];
    h = padY * 2.0f + rowY[nRows];
}

    void drawSelf() override {
        //update layout before drawing
        layout();
        if (drawBackground) {
            mat4 trans_matrix = translate(x, (float)hh - h - y, 0.0f);
            mat4 scale_matrix = scale(w, h, 1.0f);
            model_matrix = trans_matrix * scale_matrix;
            draw_solid_hud_quad(color);
        }

    }//end draw()

};//end Panel

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

//Button widget for interactivity
struct Button : Widget {
    string label;
    float textSize = HUD_FONT;
    std::function<void()> onPress;
    vec4 color = HUD_GRAY;
    vec4 highlightedColor = HUD_HIGHLIGHTED;

    //constructor (no label specified)
    Button(float x, float y, float w, float h, std::function<void()> onPress, const string& label = "" )
    : Widget(x, y, w, h), onPress(onPress), label(label) {
        fit();
        if (prefW <= 0.0f) prefW = w;   // explicit size, no label
        if (prefH <= 0.0f) prefH = h;
    }



    void fit() {
        if (label != "") {
            w = text_width(label, textSize) + HUD_PAD_X * 2.0f;
            if (w < 48.0f) w = 48.0f;
            h = textSize + HUD_PAD_Y * 2.0f + 4.0f;
            prefW = w;
            prefH = h;
        }
    }

    void drawSelf() override {
        model_matrix = mat4().identity();
        mat4 scale_matrix, trans_matrix = mat4().identity();


        scale_matrix = scale(w, h, 1.0f);
        trans_matrix = translate(x, (float)hh - h - y, 0.0f);//move to top left for more intuitive positioning increase in y goes down
        model_matrix = trans_matrix * scale_matrix;
        draw_solid_hud_quad(hovered ? highlightedColor: color);

        draw_string(label, x + HUD_PAD_X, y + HUD_PAD_Y, textSize);
    }

    void on_click(float mx, float my) override {
        if (onPress) onPress();
    }

};//end Button

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

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

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

//Label for labeling UI sections
struct Label : Widget {
    string label;
    float textSize = HUD_FONT;
    vec4 color = HUD_GRAY;

    Label(float x, float y, float w, float h, const string& label)
    : Widget(x, y, w, h), label(label) {
        fit();
    }

    void setText(string n_text) {
        label = n_text;
        fit();
    }

    void fit() {
        w = text_width(label, textSize) + HUD_PAD_X * 2.0f;
        h = textSize + HUD_PAD_Y * 2.0f + 4.0f;
        prefW = w;   // what the text actually needs
        prefH = h;
    }

    void drawSelf() override {
        model_matrix = mat4().identity();
        mat4 scale_matrix, trans_matrix = mat4().identity();

        scale_matrix = scale(w, h, 1.0f);
        trans_matrix = translate(x, (float)hh - h - y, 0.0f);
        model_matrix = trans_matrix * scale_matrix;
        draw_solid_hud_quad(color);

        draw_string(label, x + 8.0f, y + 6.0f, textSize);
    }

};//end Label

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

struct Section : Widget {
    string label;
    bool collapsed = false;
    Panel* content;
    float headerH = 0.0f;
    vec4 color = HUD_GRAY;
    vec4 highlightedColor = HUD_HIGHLIGHTED;

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

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

struct Dropdown : Widget {
    string label;
    vector<string> options;
    int* selected = nullptr;   // binds to an int (index into options)
    bool open = false;
    bool closing = false;
    bool enabled = true;
    float textSize = HUD_FONT;
    float labelW = 0.0f;
    vec4 color = HUD_GRAY;
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

    void setEnabled(bool on) {
        enabled = on;
        if (!enabled)
            closePopup();
    }

    void setOptions(const vector<string>& newOptions) {
        options = newOptions;
        //guard if no popup
        if (popup) {
            // delete the old option buttons
            for (auto& cell : popup->cells) {
                if (cell.widget) {
                    popup->removeChild(cell.widget);
                    delete cell.widget;
                }
            }
            popup->cells.clear();
            buildOptions();
            if (open) popup->show(); else popup->hide();
        }

        resize();
    }

    void buildOptions() {
        if (popup) {
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

    void drawSelf() override {
        model_matrix = translate(x, (float)hh - h - y, 0.0f) * scale(w, h, 1.0f);
        draw_solid_hud_quad(color);
        draw_string(label, x + HUD_PAD_X, y + HUD_PAD_Y, textSize);

        float boxX = x + labelW;
        float boxW = w - labelW - HUD_PAD_X;
        model_matrix = translate(boxX, (float)hh - (h - 8.0f) - (y + 4.0f), 0.0f)
                     * scale(boxW, h - 8.0f, 1.0f);
        draw_solid_hud_quad(enabled ? selectedColor: disabledColor);
        draw_string(currentText(), boxX + 6.0f, y + 6.0f, textSize, ALT_TEXT_COLOR);
        draw_string(open ? "^" : "v", boxX + boxW - 22.0f, y + 6.0f, textSize, ALT_TEXT_COLOR);
    }

    void openPopup() {
        if (!popup) {
            popup = new Panel(x + labelW, y + h);
            popup->setPriority(1);
            buildOptions();
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

    void on_click(float mx, float my) override {
        if (!enabled) return;
        if (!open) {
            open = true;
            openPopup();
        } else {
            closePopup();
        }
    }

};

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

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
        float needed = w;
        if (maxScroll > 0.0f)
            needed += scrollbarWidth;
        w = std::max(needed, assignedW);   // parent stretch wins; bar is not re-added

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




//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------

struct FileBrowser : Widget {
    string currentPath;
    bool open = false;

    vector<string> allowedExtensions;

    Panel* content = nullptr;       // the visible panel
    Label* pathLabel = nullptr;
    ScrollPanel* listPanel = nullptr;     // holds the entry buttons
    Button* closeButton = nullptr;
    Label* selectedLabel = nullptr;
    Label* filterLabel = nullptr;
    string selectedPath;

    // Optional: callback when a file is chosen
    std::function<void(const string& path)> onFileSelected;

    FileBrowser(float x, float y)
            : Widget(x, y, 0, 0) {
        content = new Panel(x, y);
        addChild(content);

        pathLabel = new Label(0, 0, 320, 32, "");
        content->add(pathLabel, 0, 0, 1, 1, Alignment::STRETCH, Alignment::START);

        closeButton = new Button(0, 0, 32, 32,
            [this]() { closeBrowser(); }, "X");
        content->add(closeButton, 1, 0, 1, 1, Alignment::END, Alignment::START);

        listPanel = new ScrollPanel(0, 0, 480.0f);
        content->add(listPanel, 0, 1, 2, 1, Alignment::STRETCH, Alignment::START);

        selectedLabel = new Label(0, 0, 200, 32, "(none)");
        content->add(selectedLabel, 0, 2, 1, 1, Alignment::STRETCH, Alignment::START);

        filterLabel = new Label(0, 0, 120, 32, "");
        content->add(filterLabel, 1, 2);

        content->hide();
        hide();
        open = false;

        const char* home = getenv("HOME");          // Linux/macOS (losers)
        if (!home) home = getenv("USERPROFILE");    // Windows (chads)
        currentPath = home ? home : fs::current_path().string();
    }

    ~FileBrowser() {
        // Panel destructor unparents; we explicitly delete what we own
        if (content) {
            // Delete entry buttons first (same pattern as Dropdown::setOptions)
            clearList();
            delete content;
            content = nullptr;
        }
    };

    //set allowed file extensions
    void setFilter(const vector<string>& extensions) {
        allowedExtensions = extensions;
        // Normalize to lowercase with leading dot
        for (auto& ext : allowedExtensions) {
            if (!ext.empty() && ext[0] != '.')
                ext = "." + ext;
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        }
        updateFilterLabel();
    }

    void updateFilterLabel() {
        if (!filterLabel) return;
        if (allowedExtensions.empty()) {
            filterLabel->setText("(any)");
            return;
        }
        string s;
        for (size_t i = 0; i < allowedExtensions.size(); i++) {
            if (i) s += " ";
            s += allowedExtensions[i];
        }
        filterLabel->setText(s);
    }

    void openBrowser(const string& startPath = "") {
        show();
        if (!startPath.empty())
            currentPath = startPath;
        open = true;
        content->show();
        refresh();
        centerOnScreen();
    }

    void closeBrowser() {
        open = false;
        content->hide();
    }

    void centerOnScreen() {
        if (!content) return;
        content->layout();
        w = content->w;
        h = content->h;
        x = (ww - w) * 0.5f;
        y = (hh - h) * 0.5f;
        content->x = x;
        content->y = y;
    }

    void clearList() {
        if (!listPanel) return;
        for (auto& cell : listPanel->cells) {
            if (cell.widget) {
                destroy_widget(cell.widget);
            }
        }
        listPanel->cells.clear();
    }

    //helper to check if file follows current extension filter
    bool passesFilter(const fs::path& p) const {
        if (allowedExtensions.empty())
            return true;   // no filter → show all files
        string ext = p.extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        for (const auto& allowed : allowedExtensions)
            if (ext == allowed)
                return true;
        return false;
    }

    void refresh() {
        if (!listPanel || !pathLabel) return;

        clearList();
        pathLabel->setText(currentPath);

        int row = 0;

        // ".." entry
        Button* up = new Button(0, 0, 280, 32,
            [this]() {
                fs::path p(currentPath);
                if (p.has_parent_path()) {
                    currentPath = p.parent_path().string();
                    refresh();
                }
            }, "..");
        listPanel->add(up, 0, row++);

        // Directory contents
        if (fs::exists(currentPath) && fs::is_directory(currentPath)) {
            std::error_code ec;
            for (fs::directory_iterator it(currentPath, fs::directory_options::skip_permission_denied, ec), end;
                 !ec && it != end; it.increment(ec)){
                const auto& entry = *it;
                string name = entry.path().filename().string();
                string full = entry.path().string();

                if (entry.is_directory()) {
                    Button* b = new Button(0, 0, 280, 32,
                        [this, full]() {
                            currentPath = full;
                            refresh();
                        }, name + "/");
                    listPanel->add(b, 0, row++);
                } else if (passesFilter(entry.path())) {
                    string ext = entry.path().extension().string();
                    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
                    if (ext == ".png" || ext == ".jpg" || ext == ".jpeg") {
                        Button* b = new Button(0, 0, 280, 32,
                            [this, full]() {
                                selectedPath = full;
                                selectedLabel->setText(fs::path(full).filename().string());
                                if (onFileSelected)
                                    onFileSelected(full);
                                closeBrowser();
                            }, name);
                        listPanel->add(b, 0, row++);
                    }
                }
            }
        }

        listPanel->layout();
        content->layout();
        w = content->w;
        h = content->h;
    }// rebuild the list of entries

    void drawSelf() override {
        //draw handled by children
    }

};


// -------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------- UTILITY FUNCTONS ------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------------------------


//util function for drawing text
void draw_string(const string& text, float x, float y, float size, vec4 color) {
    float scale = size / 32.0f;   // bake height is 32

    glUseProgram(hud_program);
    glUniformMatrix4fv(hud_proj_mat_loc, 1, GL_FALSE, proj_matrix);
    glUniformMatrix4fv(hud_camera_mat_loc, 1, GL_FALSE, camera_matrix);
    glUniform2f(hud_uv_scale_loc, 1.0f, 1.0f);
    glUniform2f(hud_uv_offset_loc, 0.0f, 0.0f);

    GLint colorLoc = glGetUniformLocation(hud_program, "textColor");
    if (colorLoc != -1)
        glUniform4fv(colorLoc, 1, color);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fontTextureID);

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
        draw_text_quad();
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

#endif //KITUI_WIDGETS_H