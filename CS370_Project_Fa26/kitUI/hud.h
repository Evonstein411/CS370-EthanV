#ifndef KITUI_HUD_H
#define KITUI_HUD_H

#include "widgets.h"

bool hud_mouse_button(float mx, float my, int button, int action);
bool hud_cursor(float mx, float my);
bool hud_char(unsigned int codepoint);
bool hud_key(int key, int action);   // returns true if UI consumed it
Widget* find_topmost(Widget* w, float mx, float my);
Widget* find_topmost_widget(float mx, float my);


inline Widget* captured = nullptr;


inline bool hud_char(const unsigned int codepoint) {
    EventScope scope;
    for (auto & widget : widgets) widget->on_char(codepoint);
    return true;
}


inline bool hud_key(const int key, const int action) {
    EventScope scope;
    bool typing = false;
    //if a field is focused we are typing
    for (const auto & widget : widgets) {
        if (const auto* field = dynamic_cast<TextFieldBase*>(widget); field && field->focused) typing = true;
    }

    //not typing
    if (!typing)
        return false;

    //commit and unfocus if enter or escape pressed after typing
    if ((key == GLFW_KEY_ENTER || key == GLFW_KEY_ESCAPE) && action == GLFW_PRESS) {
        for (const auto & widget : widgets) {
            auto* field = dynamic_cast<TextFieldBase*>(widget);
            if (field && field->focused) field->commit();
            if (field) field->focused = false;
        }
        return true;
    }

    //call on keys
    for (const auto & widget : widgets)
        widget->on_key(key, action);
    return true;

}

inline bool hud_cursor(const float mx, const float my) {
    EventScope scope;
    const Widget* top = find_topmost_widget(mx, my);

    for (Widget* w : widgets) {
        w->hovered = w == top;
        if (w->hovered)
            w->on_hover(mx, my);
    }

    if (captured)
        captured->on_drag(mx, my);

    return true;
}


inline bool hud_mouse_button(const float mx, const float my, const int button, const int action) {
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
    for (const auto & widget : widgets) {
        auto* field = dynamic_cast<TextFieldBase*>(widget);
        if (!field) continue;
        if (field->focused && field != top)
            field->commit();
        field->focused = field->editable && field == top;
    }

    //close dropdowns
    for (const auto & widget : widgets) {
        if (auto* dd = dynamic_cast<Dropdown*>(widget); dd && dd->open && !dd->contains(mx, my))
            dd->close_popup();
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


inline void draw_widgets() {
    //don't need to check depth for ui
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    //NOTE: IF YOU WANT TRANSPARENCY OF HUD ELEMENTS MAKE SURE ALPHA BLENDING IS ON

    //switch to pixel based ortho camera, store camera data pre hud draw
    const mat4 sceneProjMatrix = projMatrix;
    const mat4 sceneCameraMatrix = cameraMatrix;
    projMatrix = ortho(0.0f, static_cast<float>(ww), 0.0f, static_cast<float>(hh), -1.0f, 1.0f);
    cameraMatrix  = mat4::identity();

    //model matrices
    modelMatrix = mat4::identity();

    //list of parents
    vector<Widget*> topLevel;
    for (Widget* w : widgets)
        if (!w->is_child())
            topLevel.push_back(w);

    //sort by priority
    std::stable_sort(topLevel.begin(), topLevel.end(),
        [](const Widget* a, const Widget* b) { return a->priority < b->priority; });

    for (Widget* w : topLevel)
        w->draw();

    //reset settings to pre hud state, restore camera
    projMatrix = sceneProjMatrix;
    cameraMatrix = sceneCameraMatrix;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);


}//end render hud


//helper function for finding topmost widget for hit testing
inline Widget* find_topmost(Widget* w, const float mx, const float my) {
    if (!w || w->hidden || w->is_effectively_hidden())
        return nullptr;

    // Children: higher priority first, then reverse creation order
    vector<Widget*> kids = w->children;
    std::stable_sort(kids.begin(), kids.end(),
        [](const Widget* a, const Widget* b) { return a->priority > b->priority; });

    for (Widget* c : kids) {
        if (Widget* hit = find_topmost(c, mx, my))
            return hit;
    }

    if (w->hit(mx, my))
        return w;
    return nullptr;
}

inline Widget* find_topmost_widget(const float mx, const float my) {
    vector<Widget*> topLevel;
    for (Widget* w : widgets)
        if (!w->is_child())
            topLevel.push_back(w);

    // Higher priority first
    std::stable_sort(topLevel.begin(), topLevel.end(),
        [](const Widget* a, const Widget* b) { return a->priority > b->priority; });

    for (Widget* w : topLevel) {
        if (Widget* hit = find_topmost(w, mx, my))
            return hit;
    }
    return nullptr;
}

//UI related draw functions randomly thrown in for now need moved/fixed
inline void draw_solid_hud_quad(const vmath::vec4 &color) {
    //setup dynamic buffer
    if (dynamicHUDColorBuffer == 0){
        glGenBuffers(1, &dynamicHUDColorBuffer);
        // allocate once to the size of the HUD quad
        std::vector<vmath::vec4> tmp(HUD_numVertices[HUDQuad], color);
        glBindBuffer(GL_ARRAY_BUFFER, dynamicHUDColorBuffer);
        glBufferData(GL_ARRAY_BUFFER, tmp.size() * sizeof(vmath::vec4), tmp.data(), GL_DYNAMIC_DRAW);
    }

    std::vector<vmath::vec4> cols(HUD_numVertices[HUDQuad], color);
    glBindBuffer(GL_ARRAY_BUFFER, dynamicHUDColorBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, cols.size() * sizeof(vmath::vec4), cols.data());

    //draw color object
    glUseProgram(colorProgram);
    glUniformMatrix4fv(colorProjMatLoc, 1, GL_FALSE, projMatrix);
    glUniformMatrix4fv(colorCamMatLoc, 1, GL_FALSE, cameraMatrix);
    glUniformMatrix4fv(colorModelMatLoc, 1, GL_FALSE, modelMatrix);

    glBindVertexArray(HUD_VAOs[HUDQuad]);
    glBindBuffer(GL_ARRAY_BUFFER, HUD_ObjBuffers[HUDQuad][PosBuffer]);
    glVertexAttribPointer(colorVPos, posCoords, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(colorVPos);

    glBindBuffer(GL_ARRAY_BUFFER, dynamicHUDColorBuffer);
    glVertexAttribPointer(colorVCol, colCoords, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(colorVCol);

    glDrawArrays(GL_TRIANGLES, 0, HUD_numVertices[HUDQuad]);
}


inline void draw_text_quad() {
    glUseProgram(hudProgram);

    glUniformMatrix4fv(hudProjMatLoc, 1, GL_FALSE, projMatrix);
    glUniformMatrix4fv(hudCameraMatLoc, 1, GL_FALSE, cameraMatrix);
    glUniformMatrix4fv(hudModelMatLoc, 1, GL_FALSE, modelMatrix);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fontTextureID);

    glBindVertexArray(HUD_VAOs[HUDTextQuad]);

    glBindBuffer(GL_ARRAY_BUFFER, HUD_ObjBuffers[HUDTextQuad][PosBuffer]);
    glVertexAttribPointer(hudVPos, posCoords, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(hudVPos);

    glBindBuffer(GL_ARRAY_BUFFER, HUD_ObjBuffers[HUDTextQuad][TexBuffer]);
    glVertexAttribPointer(hudVTex, texCoords, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(hudVTex);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

#endif