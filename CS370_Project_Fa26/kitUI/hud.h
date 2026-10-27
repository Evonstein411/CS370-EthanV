#ifndef KITUI_HUD_H
#define KITUI_HUD_H

#include "widgets.h"

bool hud_mouse_button(float mx, float my, int button, int action);
bool hud_cursor(float mx, float my);
bool hud_char(unsigned int codepoint);
bool hud_key(int key, int action);   // returns true if UI consumed it


bool hud_cursor(float mx, float my) {
    for (size_t i = 0; i < widgets.size(); i++) {
        widgets[i]->hovered = widgets[i]->hit(mx, my);
        widgets[i]->on_hover(mx, my);
    }
    return true;
}


bool hud_char(unsigned int codepoint) {
    for (size_t i = 0; i < widgets.size(); i++) widgets[i]->on_char(codepoint);
    return true;
}


bool hud_key(int key, int action) {
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


bool hud_mouse_button(float mx, float my, int button, int action) {
    //not left click
    if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_PRESS)
        return false;

    //check if clicked on widget
    int hitIndex = -1;
    for (int i = (int)widgets.size() - 1; i >= 0; i--) {
        if (widgets[i]->hit(mx, my)) {
            hitIndex = i;
            break;
        }
    }

    //check text fields for focus
    for (size_t i = 0; i < widgets.size(); i++) {
        TextFieldBase* field = dynamic_cast<TextFieldBase*>(widgets[i]);
        if(field) {
            //if focused and not clicked commit
            if (field->focused && (int)i != hitIndex)
                field->commit();
            field->focused = (field->editable && (int)i == hitIndex);
        }
    }

    //check dropdowns for closing
    for (size_t i = 0; i < widgets.size(); i++) {
        Dropdown* dd = dynamic_cast<Dropdown*>(widgets[i]);
        if (dd && dd->open && !dd->contains(mx, my))
            dd->closePopup();
    }

    //hit something call on click
    if (hitIndex >= 0) {
        widgets[hitIndex]->on_click();
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


    for (size_t i = 0; i < widgets.size(); i++) {
        if(!(widgets[i]->isChild())){widgets[i]->draw();}
    }

    //reset settings to pre hud state, restore camera
    proj_matrix = scene_proj_matrix;
    camera_matrix = scene_camera_matrix;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);


}//end render hud

#endif