#include "kitUI/hud.h"


//selected object (exists in DesertFox.cpp)
extern SceneObject* selectedObject;

//globals
int selectedMesh = -1;
int selectedType = -1;
int selectedTexture = -1;
string name_string = "";

//file browsers
FileBrowser* texture_file_browser = nullptr;
FileBrowser* mesh_file_browser = nullptr;
FileBrowser* scene_file_browser = nullptr;

//sections
Section* material_section = nullptr;
Section* object_section = nullptr;
Section* solid_color_section = nullptr;
Section* texture_section = nullptr;
Section* transform_section = nullptr;
//canvas
Canvas* topbar = nullptr;

//panels
Panel* topbar_panel = nullptr;
Panel* object_panel = nullptr;
Panel* settings_panel = nullptr;
Panel* material_panel = nullptr;
Panel* solid_color_panel = nullptr;
Panel* texture_panel = nullptr;
Panel* transform_panel = nullptr;

Panel* scene_graph = nullptr;

//dropdowns
Dropdown* type_dropdown = nullptr;
Dropdown* model_dropdown = nullptr;
Dropdown* texture_dropdown = nullptr;

//labels
Label* ambient_label = nullptr;
Label* diffuse_label = nullptr;
Label* specular_label = nullptr;
Label* shininess_label = nullptr;
Label* translation_label = nullptr;
Label* rotation_label = nullptr;
Label* scale_label = nullptr;

//buttons
Button* light_button = nullptr;
Button* add_texture_button = nullptr;
Button* add_model_button = nullptr;
Button* add_object_button = nullptr;
Button* load_scene_button = nullptr;
Button* save_scene_button = nullptr;

//button functions
void toggle_lights();
void add_texture();
void add_model();
void add_object();
void load_scene();
void save_scene();

//text fields
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

TextField<float>* color_r_field = nullptr;
TextField<float>* color_g_field = nullptr;
TextField<float>* color_b_field = nullptr;
TextField<float>* color_a_field = nullptr;

TextField<float>* trans_x_field = nullptr;
TextField<float>* trans_y_field = nullptr;
TextField<float>* trans_z_field = nullptr;

TextField<float>* rot_x_field = nullptr;
TextField<float>* rot_y_field = nullptr;
TextField<float>* rot_z_field = nullptr;

TextField<float>* scale_x_field = nullptr;
TextField<float>* scale_y_field = nullptr;
TextField<float>* scale_z_field = nullptr;

//field variables
float trans_x = 0.0f;
float trans_y = 0.0f;
float trans_z = 0.0f;

float rot_x = 0.0f;
float rot_y = 0.0f;
float rot_z = 0.0f;

float scale_x = 0.0f;
float scale_y = 0.0f;
float scale_z = 0.0f;



//forward decs
void update_fields();
void rebuild_scene_graph();
void select_object(SceneObject* obj);

void render_hud() {
    draw_widgets();
}

void init_hud() {

    Canvas* topbar = new Canvas(0, 0, (float)ww, 64);

    light_button = new Button(0.0f, 0.0f, 240.0f, 40.0f, toggle_lights, "TOGGLE LIGHTS");
    topbar->add(light_button, ww-240.0f, 4.0f);

    add_object_button = new Button(0.0f, 0.0f, 160.0f, 40.0f, add_object, "Add Object");
    topbar->add(add_object_button, 8.0f, 4.0f);

    load_scene_button = new Button(0.0f, 0.0f, 160.0f, 40.0f, load_scene, "Load Scene");
    topbar->add(load_scene_button, 168.0f, 4.0f);

    save_scene_button = new Button(0.0f, 0.0f, 160.0f, 40.0f, save_scene, "Save Scene");
    topbar->add(save_scene_button, 336.0f, 4.0f);

    scene_graph = new Panel(ww - 200.0f, 80.0f);
    rebuild_scene_graph();


    //main panel owns all object detail related widgets
    object_panel = new Panel(20.0f, 90.0f);

    //object settings section (name, type, etc.)
    object_section = new Section(0.0f, 0.0f, "Object");
    object_panel->add(object_section, 0, 0, 1, 1, Alignment::STRETCH, Alignment::START);

    //material section
    material_section = new Section(0.0f, 0.0f, "Material");
    object_panel->add(material_section, 0, 1, 1, 1, Alignment::STRETCH, Alignment::START);

    //color section
    solid_color_section = new Section(0.0f, 0.0f, "Solid Color");
    object_panel->add(solid_color_section, 0, 2, 1, 1, Alignment::STRETCH, Alignment::START);

    //texture section
    texture_section = new Section(0.0f, 0.0f, "Texture");
    object_panel->add(texture_section, 0, 3, 1, 1, Alignment::STRETCH, Alignment::START);

    //transform section
    transform_section = new Section(0.0f, 0.0f, "Transformation");
    object_panel->add(transform_section, 0, 4, 1, 1, Alignment::STRETCH, Alignment::START);


    //setting panel
    settings_panel = new Panel(20.0f, 90.0f);
    object_section->add(settings_panel);

    name_field = new TextField<string>(0.0f, 0.0f, 65.0f, 48.0f, "Name:", 20.0f, false, &name_string);
    settings_panel->add(name_field, 0, 0);

    model_dropdown = new Dropdown(0.0f, 0.0f, 1.0f, 1.0f, "Mesh:", meshNames, selectedMesh);
    settings_panel->add(model_dropdown, 0, 1);

    add_model_button = new Button(0.0f, 0.0f, 240.0f, 40.0f, add_model, "Add Model");
    settings_panel->add(add_model_button, 1, 1);

    type_dropdown = new Dropdown(0.0f, 0.0f, 1.0f, 1.0f, "Draw Type:", drawTypeNames, selectedType);
    settings_panel->add(type_dropdown, 0, 2);

    scene_file_browser = new FileBrowser();
    scene_file_browser->setFilter({".df"}); //need to come up with extension for a scene using .df for now
    scene_file_browser->onFileSelected = [](const string& path){};


    mesh_file_browser = new FileBrowser();
    mesh_file_browser->setFilter({".obj"});
    mesh_file_browser->onFileSelected = [](const string& path) {
        int index = add_model_file(path);
        if (index < 0) return;

        selectedMesh = index;
        if (model_dropdown)
            model_dropdown->setOptions(meshNames);

        if (selectedObject) {
            selectedObject->mesh = (GLuint)index;
            if (selectedObject->drawType == SOLID_COLOR && selectedObject->hasColor)
                build_solid_color_buffer(numVertices[index],selectedObject->color,selectedObject->colorBuffer);
        }
    };

    //material panel
    material_panel = new Panel(20.0f, 170.0f);
    material_section->add(material_panel);

    //ambient
    ambient_label = new Label(0.0f, 0.0f, 130.0f, 48.0f, "ambient");
    material_panel->add(ambient_label, 0, 0, 1, 1, Alignment::STRETCH, Alignment::START);

    ambient_r_field = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "R:", 4.0f, true, &ambient_r);
    material_panel->add(ambient_r_field, 1, 0);

    ambient_g_field = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "G:", 4.0f, true, &ambient_g);
    material_panel->add(ambient_g_field, 2, 0);

    ambient_b_field = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "B:", 4.0f, true, &ambient_b);
    material_panel->add(ambient_b_field, 3, 0);

    ambient_a_field =new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "A:", 4.0f, true, &ambient_a);
    material_panel->add(ambient_a_field, 4, 0);

    //diffuse
    diffuse_label= new Label(0.0f, 0.0f, 130.0f, 48.0f, "diffuse");
    material_panel->add(diffuse_label, 0, 1, 1, 1, Alignment::STRETCH, Alignment::START);

    diffuse_r_field = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "R:", 4.0f, true, &diffuse_r);
    material_panel->add(diffuse_r_field, 1, 1);

    diffuse_g_field = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "G:", 4.0f, true, &diffuse_g);
    material_panel->add(diffuse_g_field, 2, 1);

    diffuse_b_field = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "B:", 4.0f, true, &diffuse_b);
    material_panel->add(diffuse_b_field, 3, 1);

    diffuse_a_field =new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "A:", 4.0f, true, &diffuse_a);
    material_panel->add(diffuse_a_field, 4, 1);


    //specular
    specular_label = new Label(0.0f, 0.0f, 130.0f, 48.0f, "specular");
    material_panel->add(specular_label, 0, 2, 1, 1, Alignment::STRETCH, Alignment::START);

    specular_r_field = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "R:", 4.0f, true, &specular_r);
    material_panel->add(specular_r_field, 1, 2);

    specular_g_field = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "G:", 4.0f, true, &specular_g);
    material_panel->add(specular_g_field, 2, 2);

    specular_b_field = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "B:", 4.0f, true, &specular_b);
    material_panel->add(specular_b_field, 3, 2);


    specular_a_field =new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "A:", 4.0f, true, &specular_a);
    material_panel->add(specular_a_field, 4, 2);

    //shininess
    shininess_label = new Label(0.0f, 0.0f, 130.0f, 48.0f, "shininess");
    material_panel->add(shininess_label, 0, 3, 1, 1, Alignment::START, Alignment::START);
    shininess_field = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, ":", 4.0f, true, &shininess);
    material_panel->add(shininess_field, 1, 3);

    //solid color panel
    solid_color_panel = new Panel(0.0f, 0.0f);
    solid_color_section->add(solid_color_panel);

    color_r_field = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "R:", 4.0f, true, &color_r);
    solid_color_panel->add(color_r_field, 0, 0);

    color_g_field = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "G:", 4.0f, true, &color_g);
    solid_color_panel->add(color_g_field, 0, 1);

    color_b_field = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "B:", 4.0f, true, &color_b);
    solid_color_panel->add(color_b_field, 0, 2);

    color_a_field =new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "A:", 4.0f, true, &color_a);
    solid_color_panel->add(color_a_field, 0, 3);

    //texture panel
    texture_panel = new Panel(0.0f, 0.0f);
    texture_section->add(texture_panel);

    texture_dropdown = new Dropdown(0.0f, 0.0f, 1.0f, 1.0f, "Texture:", textureNames, selectedTexture);
    texture_panel->add(texture_dropdown, 0, 0);

    add_texture_button = new Button(0.0f, 0.0f, 240.0f, 40.0f, add_texture, "Add Texture");
    texture_panel->add(add_texture_button, 1, 0, 1, 1, Alignment::START, Alignment::START);

    texture_file_browser = new FileBrowser();
    texture_file_browser->setFilter({".png", ".jpg", ".jpeg"});
    texture_file_browser->onFileSelected = [](const string& texturePath) {
        int tex_index = add_texture_file(texturePath);
        if (tex_index < 0) return;

        selectedTexture = tex_index;
        if (texture_dropdown)
            texture_dropdown->setOptions(textureNames);

        if (selectedObject) {
            selectedObject->drawType = TEXTURE;
            selectedObject->texture = TextureIDs[tex_index];
        }
    };

    transform_panel = new Panel(0.0f, 0.0f);
    transform_section->add(transform_panel);

    translation_label = new Label(0.0f, 0.0f, 130.0f, 48.0f, "translation");
    transform_panel->add(translation_label, 0, 0, 1, 1, Alignment::STRETCH, Alignment::START);

    trans_x_field = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "X:", 4.0f, true, &trans_x);
    transform_panel->add(trans_x_field, 1, 0);

    trans_y_field = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "Y:", 4.0f, true, &trans_y);
    transform_panel->add(trans_y_field, 2, 0);

    trans_z_field = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "Z:", 4.0f, true, &trans_z);
    transform_panel->add(trans_z_field, 3, 0);

    rotation_label = new Label(0.0f, 0.0f, 130.0f, 48.0f, "rotation");
    transform_panel->add(rotation_label, 0, 1, 1, 1, Alignment::STRETCH, Alignment::START);

    rot_x_field = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "X:", 4.0f, true, &rot_x);
    transform_panel->add(rot_x_field, 1, 1);

    rot_y_field = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "Y:", 4.0f, true, &rot_y);
    transform_panel->add(rot_y_field, 2, 1);

    rot_z_field = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "Z:", 4.0f, true, &rot_z);
    transform_panel->add(rot_z_field, 3, 1);

    scale_label = new Label(0.0f, 0.0f, 130.0f, 48.0f, "scale");
    transform_panel->add(scale_label, 0, 2, 1, 1, Alignment::STRETCH, Alignment::START);

    scale_x_field = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "X:", 4.0f, true, &scale_x);
    transform_panel->add(scale_x_field, 1, 2);

    scale_y_field = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "Y:", 4.0f, true, &scale_y);
    transform_panel->add(scale_y_field, 2, 2);

    scale_z_field = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "Z:", 4.0f, true, &scale_z);
    transform_panel->add(scale_z_field, 3, 2);




    update_fields();

}//end init hud

void set_section_visibility(DrawType type) {
    if (material_section)    material_section->setHidden(type != MATERIAL);
    if (solid_color_section) solid_color_section->setHidden(type != SOLID_COLOR);
    if (texture_section)     texture_section->setHidden(type != TEXTURE);
}

void sync_material_to_fields(const MaterialProperties& mat) {
    ambient_r = mat.ambient[0];  ambient_g = mat.ambient[1];
    ambient_b = mat.ambient[2];  ambient_a = mat.ambient[3];
    diffuse_r = mat.diffuse[0];  diffuse_g = mat.diffuse[1];
    diffuse_b = mat.diffuse[2];  diffuse_a = mat.diffuse[3];
    specular_r = mat.specular[0]; specular_g = mat.specular[1];
    specular_b = mat.specular[2]; specular_a = mat.specular[3];
    shininess = mat.shininess;

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
    if (shininess_field)  shininess_field->setText(to_string(shininess));
}

void sync_material_from_fields(MaterialProperties& mat) {
    mat.ambient  = vec4(ambient_r, ambient_g, ambient_b, ambient_a);
    mat.diffuse  = vec4(diffuse_r, diffuse_g, diffuse_b, diffuse_a);
    mat.specular = vec4(specular_r, specular_g, specular_b, specular_a);
    mat.shininess = shininess;
}

void sync_color_to_fields(const vec4& color) {
    color_r = color[0]; color_g = color[1];
    color_b = color[2]; color_a = color[3];
    if (color_r_field) color_r_field->setText(to_string(color_r));
    if (color_g_field) color_g_field->setText(to_string(color_g));
    if (color_b_field) color_b_field->setText(to_string(color_b));
    if (color_a_field) color_a_field->setText(to_string(color_a));
}

void sync_color_from_fields(vec4& color) {
    color = vec4(color_r, color_g, color_b, color_a);
}

void sync_transform_to_fields(vec3 translate, vec3 rotate, vec3 scale) {
    trans_x = translation[0]; trans_y = translation[1]; trans_z = translation[2];
    rot_x = rotation[0]; rot_y = rotation[1]; rot_z = rotation[2];
    scale_x = scale[0]; scale_y = scale[1]; scale_z = scale[2];

    if (trans_x_field) trans_x_field->setText(to_string(trans_x));
    if (trans_y_field) trans_y_field->setText(to_string(trans_y));
    if (trans_z_field) trans_z_field->setText(to_string(trans_z));

    if (rot_x_field) rot_x_field->setText(to_string(rot_x));
    if (rot_y_field) rot_y_field->setText(to_string(rot_y));
    if (rot_z_field) rot_z_field->setText(to_string(rot_z));

    if (scale_x_field) scale_x_field->setText(to_string(scale_x));
    if (scale_y_field) scale_y_field->setText(to_string(scale_y));
    if (scale_z_field) scale_z_field->setText(to_string(scale_z));

}

void sync_transform_from_fields(vec3 translate, vec3 rotate, vec3 scale) {
    selectedObject->position = translate;
    selectedObject->rotation = rotate;
    selectedObject->scale = scale;
}

int find_texture_index(GLuint texID) {
    for (int i = 0; i < (int)TextureIDs.size(); ++i)
        if (TextureIDs[i] == texID) return i;
    return -1;
}



void update_fields() {
    if (!selectedObject) {
        name_field->setText("NONE");
        name_field->setEditable(false);
        type_dropdown->setEnabled(false);
        model_dropdown->setEnabled(false);
        selectedType = -1;
        selectedTexture = -1;
        selectedMesh = -1;
        transform_section->hide();
        set_section_visibility(static_cast<DrawType>(-1)); // hides all
        return;
    }

    if (selectedObject->mesh == -1) {
        type_dropdown->setEnabled(false);
    }else {
        type_dropdown->setEnabled(true);
    }


    name_field->setEditable(true);
    name_string = selectedObject->name;
    name_field->setText(name_string);

    model_dropdown->setEnabled(true);
    selectedMesh = (int)selectedObject->mesh;

    selectedType = selectedObject->drawType;
    set_section_visibility(selectedObject->drawType);

    transform_section->show();
    sync_transform_to_fields(selectedObject->position, selectedObject->rotation, selectedObject->scale);

    // Refresh texture list only when the texture section is relevant
    if (texture_dropdown && selectedObject->drawType == TEXTURE) {
        texture_dropdown->setOptions(textureNames);
        selectedTexture = find_texture_index(selectedObject->texture);
    } else {
        selectedTexture = -1;
    }

    if (selectedObject->drawType == MATERIAL) {
        sync_material_to_fields(DynamicMaterials[selectedObject->material]);
    } else if (selectedObject->drawType == SOLID_COLOR) {
        sync_color_to_fields(selectedObject->color);
    }
}


void update_from_fields() {
    if (!selectedObject) return;


    sync_transform_from_fields({trans_x, trans_y, trans_z}, {rot_x, rot_y, rot_z}, {scale_x, scale_y, scale_z});

    //mesh
    if (selectedMesh >= 0 && selectedMesh < (int)meshNames.size() && selectedObject->mesh != selectedMesh) {
        selectedObject->mesh = selectedMesh;
        if (selectedObject->drawType == SOLID_COLOR && selectedObject->hasColor) {
            build_solid_color_buffer(numVertices[selectedObject->mesh],
                                     selectedObject->color,
                                     selectedObject->colorBuffer);
        }
    }

    if (selectedObject->mesh == -1) {
        type_dropdown->setEnabled(false);
    }else {
        type_dropdown->setEnabled(true);
    }

    // Name
    if (name_field && selectedObject->name != name_string) {
        selectedObject->name = name_string;
        rebuild_scene_graph();
    }

    // Type change
    if (selectedType >= 0 && selectedObject->drawType != static_cast<DrawType>(selectedType)) {
        selectedObject->initializeType(static_cast<DrawType>(selectedType));
        selectedObject->drawType = static_cast<DrawType>(selectedType);
        update_fields();          // refresh UI for the new type
        return;                   // avoid writing stale field values this frame
    }



    // Per-type writes
    if (selectedObject->drawType == MATERIAL) {
        sync_material_from_fields(DynamicMaterials[selectedObject->material]);
    } else if (selectedObject->drawType == SOLID_COLOR) {
        vec4 c;
        sync_color_from_fields(c);
        selectedObject->setColor(c);
    } else if (selectedObject->drawType == TEXTURE) {
        if (selectedTexture >= 0 && selectedTexture < (int)TextureIDs.size())
            selectedObject->texture = TextureIDs[selectedTexture];
    }
}

void select_object(SceneObject* obj) {
    selectedObject = obj;
    update_fields();
}

void delete_selected_object() {
    if (!selectedObject) return;

    // remove from the scene list
    auto it = std::find(objects.begin(), objects.end(), selectedObject);
    if (it != objects.end())
        objects.erase(it);

    delete selectedObject;
    selectedObject = nullptr;

    update_fields();          // clear the detail panel
    rebuild_scene_graph();    // refresh the list
}

void rebuild_scene_graph() {
    if (!scene_graph) return;

    for (auto& cell : scene_graph->cells) {
        if (cell.widget)
            destroy_widget(cell.widget);
    }
    scene_graph->cells.clear();

    Label* title = new Label(0, 0, 240.0f, 32.0f, "Scene Graph");
    scene_graph->add(title, 0, 0);

    for (int i = 0; i < (int)objects.size(); ++i) {
        SceneObject* obj = objects[i];
        Button* b = new Button(0, 0, 240.0f, 36.0f,
            [obj]() { select_object(obj); },
            obj->name);
        scene_graph->add(b, 0, i + 1);
    }
}

//-----------------------------------Button Functions--------------------------------------
//----------------------all function used by buttons defined below-------------------------

void toggle_lights() {
    lightOn[WhitePointLight] = !lightOn[WhitePointLight];
}

void add_texture(){
    texture_file_browser->openBrowser();
}

void add_model() {
    mesh_file_browser->openBrowser();
}

void add_object() {
    string name = "object" + std::to_string(objects.size() + 1);
    SceneObject* obj = new SceneObject(name);   // empty
    select_object(obj);
    rebuild_scene_graph();
    update_fields();
}

void load_scene() {
    scene_file_browser->openBrowser();
}

void save_scene() {
    //TODO
}