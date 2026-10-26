
#include "kitUI/hud.h"

//globals
int selectedType = -1;
int selectedTexture = -1;
string name_string = "";

//sections
Section* material_section = nullptr;
Section* object_section = nullptr;
Section* solid_color_section = nullptr;
Section* texture_section = nullptr;

//panels
Panel* object_panel = nullptr;
Panel* settings_panel = nullptr;
Panel* material_panel = nullptr;
Panel* solid_color_panel = nullptr;
Panel* texture_panel = nullptr;

//dropdowns
Dropdown* type_dropdown = nullptr;
Dropdown* texture_dropdown = nullptr;

//labels
Label* ambient_label = nullptr;
Label* diffuse_label = nullptr;
Label* specular_label = nullptr;
Label* shininess_label = nullptr;

//buttons
Button* light_button = nullptr;
Button* add_texture_button = nullptr;

//button functions
void toggle_lights();
void add_texture();

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


void render_hud() {
    draw_widgets();
}

void init_hud() {
    light_button = new Button(20.0f, 20.0f, 240.0f, 40.0f, toggle_lights, "TOGGLE LIGHTS");

    //main panel owns all object detail related widgets
    object_panel = new Panel(20.0f, 90.0f);

    //object settings section (name, type, etc.)
    object_section = new Section(0.0f, 0.0f, "Object Settings");
    object_panel->add(object_section, 0, 0);

    //material section
    material_section = new Section(0.0f, 0.0f, "Material Details");
    object_panel->add(material_section, 0, 1);

    //color section
    solid_color_section = new Section(0.0f, 0.0f, "Solid Color");
    object_panel->add(solid_color_section, 0, 2);

    //texture section
    texture_section = new Section(0.0f, 0.0f, "Texture");
    object_panel->add(texture_section, 0, 3);


    //setting panel
    settings_panel = new Panel(20.0f, 90.0f);
    object_section->add(settings_panel);

    name_field = new TextField<string>(0.0f, 0.0f, 65.0f, 48.0f, "Name:", 20.0f, false, &name_string);
    settings_panel->add(name_field, 0, 0);

    type_dropdown = new Dropdown(0.0f, 0.0f, 1.0f, 1.0f, "Draw Type:", drawTypeNames, selectedType);
    settings_panel->add(type_dropdown, 0, 1);

    //material panel
    material_panel = new Panel(20.0f, 170.0f);
    material_section->add(material_panel);

    //ambient
    ambient_label = new Label(0.0f, 0.0f, 130.0f, 48.0f, "ambient");
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
    diffuse_label= new Label(0.0f, 0.0f, 130.0f, 48.0f, "diffuse");
    material_panel->add(diffuse_label, 0, 1);

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
    material_panel->add(specular_label, 0, 2);

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
    material_panel->add(shininess_label, 0, 3);
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
    texture_panel->add(add_texture_button, 0, 1);


    update_fields();

}//end init hud


void update_fields() {
    if (!selectedObject) {
        name_field->setText("NONE");
        name_field->setEditable(false);
        selectedType = -1;
        if(material_section) material_section->hide();
        if(solid_color_section) solid_color_section->hide();
        if(texture_section) texture_section->hide();

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
    if(type_dropdown) {
        selectedType = selectedObject->drawType;
    }

    if (selectedObject->drawType == MATERIAL) {
        material_section->show();
        solid_color_section->hide();
        texture_section->hide();


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
    }else if(selectedObject->drawType == SOLID_COLOR){
        material_section->hide();
        texture_section->hide();
        solid_color_section->show();

        vec4 color = selectedObject->color;
        color_r = color[0];
        color_g = color[1];
        color_b = color[2];
        color_a = color[3];

        if (color_r_field) color_r_field->setText(to_string(color_r));
        if (color_g_field) color_g_field->setText(to_string(color_g));
        if (color_b_field) color_b_field->setText(to_string(color_b));
        if (color_a_field) color_a_field->setText(to_string(color_a));
    }else if(selectedObject -> drawType == TEXTURE) {
        material_section->hide();
        solid_color_section->hide();
        texture_section->show();


    }

}//end update fields

void update_from_fields() {
    if (selectedObject) {

        if(selectedType >= 0 ) {
            if(selectedObject->drawType != static_cast<DrawType>(selectedType)) {
                selectedObject->initializeType(static_cast<DrawType>(selectedType));
                selectedObject->drawType = static_cast<DrawType>(selectedType);
                update_fields();
            }
            selectedObject->drawType = static_cast<DrawType>(selectedType);
        }

        //update material values from fields
        if (selectedObject->drawType == MATERIAL) {
            MaterialProperties& mat = DynamicMaterials[selectedObject->material];
            mat.ambient  = vec4(ambient_r, ambient_g, ambient_b, ambient_a);
            mat.diffuse  = vec4(diffuse_r, diffuse_g, diffuse_b, diffuse_a);
            mat.specular = vec4(specular_r, specular_g, specular_b, specular_a);
            mat.shininess = shininess;
        }

        if (selectedObject->drawType == SOLID_COLOR) {
            vec4 new_color = {color_r, color_g, color_b, color_a};
            selectedObject->setColor(new_color);
        }


        if(name_field) {
            selectedObject->name = name_string;
        }


    }//if selected object

}//end update from fields


//-----------------------------------Button Functions--------------------------------------
//----------------------all function used by buttons defined below-------------------------

void toggle_lights() {
    lightOn[WhitePointLight] = !lightOn[WhitePointLight];
}

void add_texture(){
    printf("Texture added");
}