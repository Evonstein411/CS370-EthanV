#include "widgets.h"


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