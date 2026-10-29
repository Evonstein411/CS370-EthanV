#ifndef KITUI_HUD_H
#define KITUI_HUD_H

#include "widgets.h"

bool hud_mouse_button(float mx, float my, int button, int action);
bool hud_cursor(float mx, float my);
bool hud_char(unsigned int codepoint);
bool hud_key(int key, int action);   // returns true if UI consumed it
Widget* find_topmost(Widget* w, float mx, float my);
Widget* find_topmost_widget(float mx, float my);


Widget* captured = nullptr;



bool hud_char(unsigned int codepoint) {
    EventScope scope;
    for (size_t i = 0; i < widgets.size(); i++) widgets[i]->on_char(codepoint);
    return true;
}


bool hud_key(int key, int action) {
    EventScope scope;
    bool typing = false;
    //if a field is focused we are typing
    for (size_t i = 0; i < widgets.size(); i++) {
        TextFieldBase* field = dynamic_cast<TextFieldBase*>(widgets[i]);
        if (field && field->focused) typing = true;
    }

    //not typing
    if (!typing)
        return false;

    //commit and unfocus if enter or escape pressed after typing
    if ((key == GLFW_KEY_ENTER || key == GLFW_KEY_ESCAPE) && action == GLFW_PRESS) {
        for (size_t i = 0; i < widgets.size(); i++) {
            TextFieldBase* field = dynamic_cast<TextFieldBase*>(widgets[i]);
            if (field && field->focused) field->commit();
            if (field) field->focused = false;
        }
        return true;
    }

    //call on keys
    for (size_t i = 0; i < widgets.size(); i++)
        widgets[i]->on_key(key, action);
    return true;

}

bool hud_cursor(float mx, float my) {
    EventScope scope;
    Widget* top = find_topmost_widget(mx, my);

    for (Widget* w : widgets) {
        w->hovered = (w == top);
        if (w->hovered)
            w->on_hover(mx, my);
    }

    if (captured)
        captured->on_drag(mx, my);

    return true;
}


bool hud_mouse_button(float mx, float my, int button, int action) {
    EventScope scope;
    //not left click
    if (button != GLFW_MOUSE_BUTTON_LEFT)
        return false;

    //release left click stop any drag logic
    if (action == GLFW_RELEASE) {
        if (captured) {
            captured->on_release();
            captured = nullptr;
        }
        return true;
    }

    if (action != GLFW_PRESS)
        return false;

    Widget* top = find_topmost_widget(mx, my);

    // Focus: only the field that *is* the topmost hit gets focus
    for (size_t i = 0; i < widgets.size(); i++) {
        TextFieldBase* field = dynamic_cast<TextFieldBase*>(widgets[i]);
        if (!field) continue;
        if (field->focused && field != top)
            field->commit();
        field->focused = (field->editable && field == top);
    }

    //close dropdowns
    for (size_t i = 0; i < widgets.size(); i++) {
        Dropdown* dd = dynamic_cast<Dropdown*>(widgets[i]);
        if (dd && dd->open && !dd->contains(mx, my))
            dd->closePopup();
    }

    if (top) {
        top->on_click(mx, my);
        if (top->grabbed) {
            captured = top;
        }
        return true;
    }
    return false;
}


void draw_widgets() {
    //don't need to check depth for ui
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    //NOTE: IF YOU WANT TRANSPARENCY OF HUD ELEMENTS MAKE SURE ALPHA BLENDING IS ON

    //switch to pixel based ortho camera, store camera data pre hud draw
    mat4 scene_proj_matrix = proj_matrix;
    mat4 scene_camera_matrix = camera_matrix;
    proj_matrix = ortho(0.0f, (float)ww, 0.0f, (float)hh, -1.0f, 1.0f);
    camera_matrix  = mat4().identity();

    //model matrices
    model_matrix = mat4().identity();
    mat4 scale_matrix = mat4().identity();
    mat4 trans_matrix = mat4().identity();


    //list of parents
    vector<Widget*> topLevel;
    for (Widget* w : widgets)
        if (!w->isChild())
            topLevel.push_back(w);

    //sort by priority
    std::stable_sort(topLevel.begin(), topLevel.end(),
        [](Widget* a, Widget* b) { return a->priority < b->priority; });

    for (Widget* w : topLevel)
        w->draw();

    //reset settings to pre hud state, restore camera
    proj_matrix = scene_proj_matrix;
    camera_matrix = scene_camera_matrix;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);


}//end render hud


//helper function for finding topmost widget for hit testing
Widget* find_topmost(Widget* w, float mx, float my) {
    if (!w || w->hidden || w->isEffectivelyHidden())
        return nullptr;

    // Children: higher priority first, then reverse creation order
    vector<Widget*> kids = w->children;
    std::stable_sort(kids.begin(), kids.end(),
        [](Widget* a, Widget* b) { return a->priority > b->priority; });

    for (Widget* c : kids) {
        if (Widget* hit = find_topmost(c, mx, my))
            return hit;
    }

    if (w->hit(mx, my))
        return w;
    return nullptr;
}

Widget* find_topmost_widget(float mx, float my) {
    vector<Widget*> topLevel;
    for (Widget* w : widgets)
        if (!w->isChild())
            topLevel.push_back(w);

    // Higher priority first
    std::stable_sort(topLevel.begin(), topLevel.end(),
        [](Widget* a, Widget* b) { return a->priority > b->priority; });

    for (Widget* w : topLevel) {
        if (Widget* hit = find_topmost(w, mx, my))
            return hit;
    }
    return nullptr;
}

#endif