// CS370 Final Project
// Fall 2026 Ethan VonStein

//includes
#include <stdio.h>
#include <vector>
#include "../common/GLFWutils.h"
#include "../common/textureutils.h"
#include "../common/objloader.h"
#include "../common/tangentspace.h"
#include "../common/vmath.h"
#include "../common/lighting.h"
#include "Globals.h"
#include "geometry.h"
#include "lights.h"
#include "materials.h"
#include "textures.h"
#include "drawObjects.h"
#include "shaders.h"
#include "hud.h"
#include "sceneobject.h"
#include "hitdetection.h"
#include "callbacks.h"

//globals
vector<Widget*> widgets;
vector<SceneObject*> objects;
SceneObject* selectedObject = nullptr;
string name_string = "";


//widgets
Section* material_section = nullptr;
Section* object_section = nullptr;


Panel* object_panel = nullptr;
Panel* material_panel = nullptr;

TextField<string>* name_field = nullptr;

TextField<float>* ambient_r_field = nullptr;
TextField<float>* ambient_g_field = nullptr;
TextField<float>* ambient_b_field = nullptr;
TextField<float>* ambient_a_field = nullptr;

TextField<float>* diffuse_r_field = nullptr;
TextField<float>* diffuse_g_field = nullptr;
TextField<float>* diffuse_b_field = nullptr;
TextField<float>* diffuse_a_field = nullptr;

TextField<float>* specular_r_field = nullptr;
TextField<float>* specular_g_field = nullptr;
TextField<float>* specular_b_field = nullptr;
TextField<float>* specular_a_field = nullptr;

TextField<float>* shininess_field = nullptr;



//functions
void display();
void render_scene();
void init_scene();
void render_hud();
void init_hud();
void update_fields();
void update_from_fields();




int main(int argc, char**argv)
{
	// Create OpenGL window
	GLFWwindow* window = CreateWindow("Desert Fox", ww, hh);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    } else {
        printf("OpenGL window successfully created\n");
    }

    // Store initial window size
    glfwGetFramebufferSize(window, &ww, &hh);

    // Register callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window,key_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetCursorPosCallback(window, cursor_callback);
    glfwSetCharCallback(window, char_callback);

    // Create geometry buffers
    build_geometry();
    // Create material buffers
    build_materials();
    // Create light buffers
    build_lights();
    // Create textures
    build_textures();
    // Create shaders
    build_shaders();
    // Create scene
    init_scene();
    // Create HUD
    init_hud();
    // Enable depth test
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    //enable opacity
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);

    // Set Initial camera position
    GLfloat x, y, z;
    x = (GLfloat)(radius*sin(deg2rad(azimuth))*sin(deg2rad(elevation)));
    y = (GLfloat)(radius*cos(deg2rad(elevation)));
    z = (GLfloat)(radius*cos(deg2rad(azimuth))*sin(deg2rad(elevation)));
    eye = vec3(x,y,z);

    // Start loop
    while ( !glfwWindowShouldClose( window ) ) {
        //update selected
        update_from_fields();
        //update dynamic materials
        build_dynamic_materials();
    	// Draw graphics
        display();
        // Update other events like input handling
        glfwPollEvents();
        // Swap buffer onto screen
        glfwSwapBuffers( window );

    }

    // Close window
    glfwTerminate();
    return 0;

}

void display( )
{
    // Declare projection and camera matrices
    proj_matrix = mat4().identity();
    camera_matrix = mat4().identity();

	// Clear window and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Compute anisotropic scaling
    GLfloat xratio = 1.0f;
    GLfloat yratio = 1.0f;
    // If taller than wide adjust y
    if (ww <= hh)
    {
        yratio = (GLfloat)hh / (GLfloat)ww;
    }
        // If wider than tall adjust x
    else if (hh <= ww)
    {
        xratio = (GLfloat)ww / (GLfloat)hh;
    }

    // DEFAULT ORTHOGRAPHIC PROJECTION
    proj_matrix = ortho(-radius*xratio, radius*xratio, -radius*yratio, radius*yratio, -100.0, 100.0);

    // Set camera matrix
    camera_matrix = lookat(eye, center, up);

    // Render objects
	render_scene();

    // Render HUD
    render_hud();

	// Flush pipeline
	glFlush();
}

void render_scene( ) {
    // Declare transformation matrices
    model_matrix = mat4().identity();
    normal_matrix = mat4().identity();

    for (size_t i = 0; i < objects.size(); i++) {
        objects[i]->draw();
    }


}

void init_scene() {
    SceneObject* sphereObject = new SceneObject(Sphere, "sphere", {0.0f, 0.0f, 0.0f}, {2.0f, 2.0f, 2.0f}, {0.0f, 0.0f, 0.0f}, baseMaterial);
    SceneObject* floor = new SceneObject(Cube, "floor", {0.0f, -3.0f, 0.0f}, {50.0f, 2.0f, 50.0f}, {0.0f, 0.0f, 0.0f}, floormat);
    SceneObject* wall = new SceneObject(Cube, "wall1", {25.0f, 3.0f, 00.0f}, {1.0f, 10.0f, 50.0f}, {0.0f, 0.0f, 0.0f}, floormat);
    SceneObject* wall2 = new SceneObject(Cube, "wall2", {-25.0f, 3.0f, 0.0f}, {1.0f, 10.0f, 50.0f}, {0.0f, 0.0f, 0.0f}, floormat);
    SceneObject* wall3 = new SceneObject(Cube, "wall3", {0.0f, 3.0f, 25.0f}, {50.0f, 10.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, floormat);
    SceneObject* wall4 = new SceneObject(Cube, "wall3", {0.0f, 3.0f, -25.0f}, {50.0f, 10.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, floormat);


}


void render_hud() {
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

void toggle_lights() {
    lightOn[WhitePointLight] = !lightOn[WhitePointLight];
}


void init_hud() {
    Button* light_button = new Button(20.0f, 20.0f, 240.0f, 40.0f, toggle_lights, "TOGGLE LIGHTS");

    //main panel owns all object detail related widgets
    object_panel = new Panel(20.0f, 90.0f);

    //object settings section (name, type, etc.)
    object_section = new Section(0.0f, 0.0f, "Object Settings");
    object_panel->add(object_section, 0, 0);

    //material section
    material_section = new Section(0.0f, 0.0f, "Material Details");
    object_panel->add(material_section, 0, 1);


    //setting panel
    Panel* settings_panel = new Panel(20.0f, 90.0f);
    object_section->add(settings_panel);

    name_field = new TextField<string>(0.0f, 0.0f, 65.0f, 48.0f, "Name:", 20.0f, false, &name_string);
    settings_panel->add(name_field, 0, 0);

    //material panel
    material_panel = new Panel(20.0f, 170.0f);
    material_section->add(material_panel);

    //ambient
    Label* ambient_label = new Label(0.0f, 0.0f, 130.0f, 48.0f, "ambient");
    material_panel->add(ambient_label, 0, 0);

    ambient_r_field = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "R:", 4.0f, true, &ambient_r);
    material_panel->add(ambient_r_field, 1, 0);

    ambient_g_field = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "G:", 4.0f, true, &ambient_g);
    material_panel->add(ambient_g_field, 2, 0);

    ambient_b_field = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "B:", 4.0f, true, &ambient_b);
    material_panel->add(ambient_b_field, 3, 0);

    ambient_a_field =new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "A:", 4.0f, true, &ambient_a);
    material_panel->add(ambient_a_field, 4, 0);

    //diffuse
    Label* diffuse_label= new Label(20.0f, 220.0f, 130.0f, 48.0f, "diffuse");
    material_panel->add(diffuse_label, 0, 1);

    diffuse_r_field = new TextField<float>(180.0f, 220.0f, 65.0f, 48.0f, "R:", 4.0f, true, &diffuse_r);
    material_panel->add(diffuse_r_field, 1, 1);

    diffuse_g_field = new TextField<float>(320.0f, 220.0f, 65.0f, 48.0f, "G:", 4.0f, true, &diffuse_g);
    material_panel->add(diffuse_g_field, 2, 1);

    diffuse_b_field = new TextField<float>(460.0f, 220.0f, 65.0f, 48.0f, "B:", 4.0f, true, &diffuse_b);
    material_panel->add(diffuse_b_field, 3, 1);

    diffuse_a_field =new TextField<float>(600.0f, 220.0f, 65.0f, 48.0f, "A:", 4.0f, true, &diffuse_a);
    material_panel->add(diffuse_a_field, 4, 1);


    //specular
    Label* specular_label = new Label(20.0f, 300.0f, 130.0f, 48.0f, "specular");
    material_panel->add(specular_label, 0, 2);

    specular_r_field = new TextField<float>(180.0f, 300.0f, 65.0f, 48.0f, "R:", 4.0f, true, &specular_r);
    material_panel->add(specular_r_field, 1, 2);

    specular_g_field = new TextField<float>(320.0f, 300.0f, 65.0f, 48.0f, "G:", 4.0f, true, &specular_g);
    material_panel->add(specular_g_field, 2, 2);

    specular_b_field = new TextField<float>(460.0f, 300.0f, 65.0f, 48.0f, "B:", 4.0f, true, &specular_b);
    material_panel->add(specular_b_field, 3, 2);


    specular_a_field =new TextField<float>(600.0f, 300.0f, 65.0f, 48.0f, "A:", 4.0f, true, &specular_a);
    material_panel->add(specular_a_field, 4, 2);

    //shininess
    Label* shininess_label = new Label(0.0f, 0.0f, 130.0f, 48.0f, "shininess");
    material_panel->add(shininess_label, 0, 3);
    shininess_field = new TextField<float>(20.0f, 380.0f, 65.0f, 48.0f, ":", 4.0f, true, &shininess);
    material_panel->add(shininess_field, 1, 3);

    update_fields();

}//end init hud


void update_fields() {
    if (!selectedObject) {
        name_field->setText("NONE");
        name_field->setEditable(false);
        if(material_panel) material_panel->hide();
        return;
    }else {
        if(material_panel) material_panel->show();
        name_field->setEditable(true);
    }
    // Name
    if (name_field) {
        name_string = selectedObject->name;
        name_field->setText(name_string);
    }

    if (selectedObject->drawType != MATERIAL) return;

    MaterialProperties& mat = DynamicMaterials[selectedObject->material];

    // Copy material → globals (so the fields stay bound to the globals)
    ambient_r = mat.ambient[0];
    ambient_g = mat.ambient[1];
    ambient_b = mat.ambient[2];
    ambient_a = mat.ambient[3];

    diffuse_r = mat.diffuse[0];
    diffuse_g = mat.diffuse[1];
    diffuse_b = mat.diffuse[2];
    diffuse_a = mat.diffuse[3];

    specular_r = mat.specular[0];
    specular_g = mat.specular[1];
    specular_b = mat.specular[2];
    specular_a = mat.specular[3];

    shininess = mat.shininess;

    // Refresh the displayed text
    if (ambient_r_field) ambient_r_field->setText(to_string(ambient_r));
    if (ambient_g_field) ambient_g_field->setText(to_string(ambient_g));
    if (ambient_b_field) ambient_b_field->setText(to_string(ambient_b));
    if (ambient_a_field) ambient_a_field->setText(to_string(ambient_a));

    if (diffuse_r_field) diffuse_r_field->setText(to_string(diffuse_r));
    if (diffuse_g_field) diffuse_g_field->setText(to_string(diffuse_g));
    if (diffuse_b_field) diffuse_b_field->setText(to_string(diffuse_b));
    if (diffuse_a_field) diffuse_a_field->setText(to_string(diffuse_a));

    if (specular_r_field) specular_r_field->setText(to_string(specular_r));
    if (specular_g_field) specular_g_field->setText(to_string(specular_g));
    if (specular_b_field) specular_b_field->setText(to_string(specular_b));
    if (specular_a_field) specular_a_field->setText(to_string(specular_a));

    if(shininess_field) shininess_field->setText(to_string(shininess));

}//end update fields

void update_from_fields() {
    if (selectedObject) {
        //update material values from fields
        if (selectedObject->drawType == MATERIAL) {
            MaterialProperties& mat = DynamicMaterials[selectedObject->material];
            mat.ambient  = vec4(ambient_r, ambient_g, ambient_b, ambient_a);
            mat.diffuse  = vec4(diffuse_r, diffuse_g, diffuse_b, diffuse_a);
            mat.specular = vec4(specular_r, specular_g, specular_b, specular_a);
            mat.shininess = shininess;
        }

        if(name_field) {
            selectedObject->name = name_string;
        }

    }//if selected object




}//end update from fields
