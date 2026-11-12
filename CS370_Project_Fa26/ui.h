#include "kitUI/hud.h"

// selected object (exists in DesertFox.cpp)
extern SceneObject* selectedObject;

// globals
int selectedMesh = -1;
int selectedType = -1;
int selectedTexture = -1;
string nameString = "";

// file browsers
FileBrowser* textureFileBrowser = nullptr;
FileBrowser* meshFileBrowser = nullptr;
FileBrowser* sceneFileBrowser = nullptr;

// sections
Section* materialSection = nullptr;
Section* objectSection = nullptr;
Section* solidColorSection = nullptr;
Section* textureSection = nullptr;
Section* transformSection = nullptr;
Section* sceneGraphSection = nullptr;

// canvas
Canvas* topbar = nullptr;

// panels
Panel* topbarPanel = nullptr;
Panel* objectPanel = nullptr;
Panel* settingsPanel = nullptr;
Panel* materialPanel = nullptr;
Panel* solidColorPanel = nullptr;
Panel* texturePanel = nullptr;
Panel* transformPanel = nullptr;
Panel* saveDialogPanel = nullptr;
Panel* sceneGraphPanel = nullptr;

// dropdowns
Dropdown* typeDropdown = nullptr;
Dropdown* modelDropdown = nullptr;
Dropdown* textureDropdown = nullptr;

// labels
Label* ambientLabel = nullptr;
Label* diffuseLabel = nullptr;
Label* specularLabel = nullptr;
Label* shininessLabel = nullptr;
Label* translationLabel = nullptr;
Label* rotationLabel = nullptr;
Label* scaleLabel = nullptr;
Label* saveSceneLabel = nullptr;

// buttons
Button* lightButton = nullptr;
Button* addTextureButton = nullptr;
Button* addModelButton = nullptr;
Button* addObjectButton = nullptr;
Button* loadSceneButton = nullptr;
Button* saveSceneButton = nullptr;
Button* saveNameButton = nullptr;
Button* cancelSaveButton = nullptr;
Button* cameraModeButton = nullptr;

// button functions
void toggle_lights();
void add_texture();
void add_model();
void add_object();
void load_scene();
void save_scene();
void confirm_save_scene();
void hide_save_dialog();
void toggle_camera_mode();

// text fields
TextField<string>* objectNameField = nullptr;
TextField<string>* saveNameField = nullptr;

TextField<float>* ambientRField = nullptr;
TextField<float>* ambientGField = nullptr;
TextField<float>* ambientBField = nullptr;
TextField<float>* ambientAField = nullptr;

TextField<float>* diffuseRField = nullptr;
TextField<float>* diffuseGField = nullptr;
TextField<float>* diffuseBField = nullptr;
TextField<float>* diffuseAField = nullptr;

TextField<float>* specularRField = nullptr;
TextField<float>* specularGField = nullptr;
TextField<float>* specularBField = nullptr;
TextField<float>* specularAField = nullptr;

TextField<float>* shininessField = nullptr;

TextField<float>* colorRField = nullptr;
TextField<float>* colorGField = nullptr;
TextField<float>* colorBField = nullptr;
TextField<float>* colorAField = nullptr;

TextField<float>* transXField = nullptr;
TextField<float>* transYField = nullptr;
TextField<float>* transZField = nullptr;

TextField<float>* rotXField = nullptr;
TextField<float>* rotYField = nullptr;
TextField<float>* rotZField = nullptr;

TextField<float>* scaleXField = nullptr;
TextField<float>* scaleYField = nullptr;
TextField<float>* scaleZField = nullptr;

// field variables
float transX = 0.0f;
float transY = 0.0f;
float transZ = 0.0f;

float rotX = 0.0f;
float rotY = 0.0f;
float rotZ = 0.0f;

float scaleX = 0.0f;
float scaleY = 0.0f;
float scaleZ = 0.0f;

string saveNameString = "";

// forward declarations
void update_fields();
void rebuild_scene_graph();
void select_object(SceneObject* obj);

void render_hud() {
    draw_widgets();
}

void init_hud() {
    // TOPBAR
    Canvas* topbar = new Canvas(0, 0, (float)ww, 64);

    lightButton = new Button(0.0f, 0.0f, 240.0f, 40.0f, toggle_lights, "TOGGLE LIGHTS");
    topbar->add(lightButton, ww - 240.0f, 4.0f);

    addObjectButton = new Button(0.0f, 0.0f, 160.0f, 40.0f, add_object, "Add Object");
    topbar->add(addObjectButton, 8.0f, 4.0f);

    loadSceneButton = new Button(0.0f, 0.0f, 160.0f, 40.0f, load_scene, "Load Scene");
    topbar->add(loadSceneButton, 168.0f, 4.0f);

    saveSceneButton = new Button(0.0f, 0.0f, 160.0f, 40.0f, save_scene, "Save Scene");
    topbar->add(saveSceneButton, 336.0f, 4.0f);

    cameraModeButton = new Button(0.0f, 0.0f, 200.0f, 40.0f, toggle_camera_mode, "ORTHO");
    topbar->add(cameraModeButton, ww - 480.0f, 4.0f);

    sceneGraphSection = new Section(ww - 200.0f, 80.0f, "Scene Graph");
    sceneGraphPanel = new Panel(ww - 200.0f, 80.0f);
    sceneGraphSection->add(sceneGraphPanel);

    rebuild_scene_graph();

    // save scene dialog
    saveDialogPanel = new Panel(0.0f, 0.0f);
    saveDialogPanel->set_priority(10);

    saveSceneLabel = new Label(0, 0, 220, 32, "Save Scene As:");
    saveDialogPanel->add(saveSceneLabel, 0, 0, 2, 1);

    saveNameField = new TextField<string>(0, 0, 220, 40, "Name:", 32, false, &saveNameString);
    saveDialogPanel->add(saveNameField, 0, 1, 2, 1);

    saveNameButton = new Button(0, 0, 100, 36, confirm_save_scene, "Save");
    saveDialogPanel->add(saveNameButton, 0, 2);

    cancelSaveButton = new Button(0, 0, 100, 36, hide_save_dialog, "Cancel");
    saveDialogPanel->add(cancelSaveButton, 1, 2);

    saveDialogPanel->hide();

    // main panel owns all object detail related widgets
    objectPanel = new Panel(20.0f, 90.0f);

    // object settings section (name, type, etc.)
    objectSection = new Section(0.0f, 0.0f, "Object");
    objectPanel->add(objectSection, 0, 0, 1, 1, Alignment::STRETCH, Alignment::START);

    // material section
    materialSection = new Section(0.0f, 0.0f, "Material");
    objectPanel->add(materialSection, 0, 1, 1, 1, Alignment::STRETCH, Alignment::START);

    // color section
    solidColorSection = new Section(0.0f, 0.0f, "Solid Color");
    objectPanel->add(solidColorSection, 0, 2, 1, 1, Alignment::STRETCH, Alignment::START);

    // texture section
    textureSection = new Section(0.0f, 0.0f, "Texture");
    objectPanel->add(textureSection, 0, 3, 1, 1, Alignment::STRETCH, Alignment::START);

    // transform section
    transformSection = new Section(0.0f, 0.0f, "Transform");
    objectPanel->add(transformSection, 0, 4, 1, 1, Alignment::STRETCH, Alignment::START);

    // setting panel
    settingsPanel = new Panel(20.0f, 90.0f);
    objectSection->add(settingsPanel);

    objectNameField = new TextField<string>(0.0f, 0.0f, 65.0f, 48.0f, "Name:", 20.0f, false, &nameString);
    settingsPanel->add(objectNameField, 0, 0);

    modelDropdown = new Dropdown(0.0f, 0.0f, 1.0f, 1.0f, "Mesh:", meshNames, selectedMesh);
    settingsPanel->add(modelDropdown, 0, 1);

    addModelButton = new Button(0.0f, 0.0f, 240.0f, 40.0f, add_model, "Add Model");
    settingsPanel->add(addModelButton, 1, 1);

    typeDropdown = new Dropdown(0.0f, 0.0f, 1.0f, 1.0f, "Draw Type:", drawTypeNames, selectedType);
    settingsPanel->add(typeDropdown, 0, 2);

    sceneFileBrowser = new FileBrowser();
    sceneFileBrowser->set_filter({".df"});
    sceneFileBrowser->onFileSelected = [](const string& path) {
        load_scene_file(path);
        rebuild_scene_graph();
    };

    meshFileBrowser = new FileBrowser();
    meshFileBrowser->set_filter({".obj"});
    meshFileBrowser->onFileSelected = [](const string& path) {
        int index = add_model_file(path);
        if (index < 0) return;

        selectedMesh = index;
        if (modelDropdown)
            modelDropdown->set_options(meshNames);

        if (selectedObject) {
            selectedObject->mesh = (GLuint)index;
            if (selectedObject->drawType == SOLID_COLOR && selectedObject->hasColor)
                build_solid_color_buffer(numVertices[index], selectedObject->color, selectedObject->colorBuffer);
        }
    };

    // material panel
    materialPanel = new Panel(20.0f, 170.0f);
    materialSection->add(materialPanel);

    // ambient
    ambientLabel = new Label(0.0f, 0.0f, 130.0f, 48.0f, "ambient");
    materialPanel->add(ambientLabel, 0, 0, 1, 1, Alignment::STRETCH, Alignment::START);

    ambientRField = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "R:", 4.0f, true, &ambientR);
    materialPanel->add(ambientRField, 1, 0);

    ambientGField = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "G:", 4.0f, true, &ambientG);
    materialPanel->add(ambientGField, 2, 0);

    ambientBField = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "B:", 4.0f, true, &ambientB);
    materialPanel->add(ambientBField, 3, 0);

    ambientAField = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "A:", 4.0f, true, &ambientA);
    materialPanel->add(ambientAField, 4, 0);

    // diffuse
    diffuseLabel = new Label(0.0f, 0.0f, 130.0f, 48.0f, "diffuse");
    materialPanel->add(diffuseLabel, 0, 1, 1, 1, Alignment::STRETCH, Alignment::START);

    diffuseRField = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "R:", 4.0f, true, &diffuseR);
    materialPanel->add(diffuseRField, 1, 1);

    diffuseGField = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "G:", 4.0f, true, &diffuseG);
    materialPanel->add(diffuseGField, 2, 1);

    diffuseBField = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "B:", 4.0f, true, &diffuseB);
    materialPanel->add(diffuseBField, 3, 1);

    diffuseAField = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "A:", 4.0f, true, &diffuseA);
    materialPanel->add(diffuseAField, 4, 1);

    // specular
    specularLabel = new Label(0.0f, 0.0f, 130.0f, 48.0f, "specular");
    materialPanel->add(specularLabel, 0, 2, 1, 1, Alignment::STRETCH, Alignment::START);

    specularRField = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "R:", 4.0f, true, &specularR);
    materialPanel->add(specularRField, 1, 2);

    specularGField = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "G:", 4.0f, true, &specularG);
    materialPanel->add(specularGField, 2, 2);

    specularBField = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "B:", 4.0f, true, &specularB);
    materialPanel->add(specularBField, 3, 2);

    specularAField = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "A:", 4.0f, true, &specularA);
    materialPanel->add(specularAField, 4, 2);

    // shininess
    shininessLabel = new Label(0.0f, 0.0f, 130.0f, 48.0f, "shininess");
    materialPanel->add(shininessLabel, 0, 3, 1, 1, Alignment::START, Alignment::START);
    shininessField = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, ":", 4.0f, true, &shininess);
    materialPanel->add(shininessField, 1, 3);

    // solid color panel
    solidColorPanel = new Panel(0.0f, 0.0f);
    solidColorSection->add(solidColorPanel);

    colorRField = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "R:", 4.0f, true, &colorR);
    solidColorPanel->add(colorRField, 0, 0);

    colorGField = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "G:", 4.0f, true, &colorG);
    solidColorPanel->add(colorGField, 0, 1);

    colorBField = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "B:", 4.0f, true, &colorB);
    solidColorPanel->add(colorBField, 0, 2);

    colorAField = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "A:", 4.0f, true, &colorA);
    solidColorPanel->add(colorAField, 0, 3);

    // texture panel
    texturePanel = new Panel(0.0f, 0.0f);
    textureSection->add(texturePanel);

    textureDropdown = new Dropdown(0.0f, 0.0f, 1.0f, 1.0f, "Texture:", textureNames, selectedTexture);
    texturePanel->add(textureDropdown, 0, 0);

    addTextureButton = new Button(0.0f, 0.0f, 240.0f, 40.0f, add_texture, "Add Texture");
    texturePanel->add(addTextureButton, 1, 0, 1, 1, Alignment::START, Alignment::START);

    textureFileBrowser = new FileBrowser();
    textureFileBrowser->set_filter({".png", ".jpg", ".jpeg"});
    textureFileBrowser->onFileSelected = [](const string& texturePath) {
        int texIndex = add_texture_file(texturePath);
        if (texIndex < 0) return;

        selectedTexture = texIndex;
        if (textureDropdown)
            textureDropdown->set_options(textureNames);

        if (selectedObject) {
            selectedObject->drawType = TEXTURE;
            selectedObject->texture = TextureIDs[texIndex];
        }
    };

    transformPanel = new Panel(0.0f, 0.0f);
    transformSection->add(transformPanel);

    translationLabel = new Label(0.0f, 0.0f, 130.0f, 48.0f, "translation");
    transformPanel->add(translationLabel, 0, 0, 1, 1, Alignment::STRETCH, Alignment::START);

    transXField = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "X:", 4.0f, true, &transX);
    transformPanel->add(transXField, 1, 0);

    transYField = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "Y:", 4.0f, true, &transY);
    transformPanel->add(transYField, 2, 0);

    transZField = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "Z:", 4.0f, true, &transZ);
    transformPanel->add(transZField, 3, 0);

    rotationLabel = new Label(0.0f, 0.0f, 130.0f, 48.0f, "rotation");
    transformPanel->add(rotationLabel, 0, 1, 1, 1, Alignment::STRETCH, Alignment::START);

    rotXField = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "X:", 4.0f, true, &rotX);
    transformPanel->add(rotXField, 1, 1);

    rotYField = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "Y:", 4.0f, true, &rotY);
    transformPanel->add(rotYField, 2, 1);

    rotZField = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "Z:", 4.0f, true, &rotZ);
    transformPanel->add(rotZField, 3, 1);

    scaleLabel = new Label(0.0f, 0.0f, 130.0f, 48.0f, "scale");
    transformPanel->add(scaleLabel, 0, 2, 1, 1, Alignment::STRETCH, Alignment::START);

    scaleXField = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "X:", 4.0f, true, &scaleX);
    transformPanel->add(scaleXField, 1, 2);

    scaleYField = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "Y:", 4.0f, true, &scaleY);
    transformPanel->add(scaleYField, 2, 2);

    scaleZField = new TextField<float>(0.0f, 0.0f, 65.0f, 48.0f, "Z:", 4.0f, true, &scaleZ);
    transformPanel->add(scaleZField, 3, 2);

    update_fields();
}

void set_section_visibility(DrawType type) {
    if (materialSection) materialSection->set_hidden(type != MATERIAL);
    if (solidColorSection) solidColorSection->set_hidden(type != SOLID_COLOR);
    if (textureSection) textureSection->set_hidden(type != TEXTURE);
}

void sync_material_to_fields(const MaterialProperties& mat) {
    ambientR = mat.ambient[0];  ambientG = mat.ambient[1];
    ambientB = mat.ambient[2];  ambientA = mat.ambient[3];
    diffuseR = mat.diffuse[0];  diffuseG = mat.diffuse[1];
    diffuseB = mat.diffuse[2];  diffuseA = mat.diffuse[3];
    specularR = mat.specular[0]; specularG = mat.specular[1];
    specularB = mat.specular[2]; specularA = mat.specular[3];
    shininess = mat.shininess;

    if (ambientRField) ambientRField->set_text(to_string(ambientR));
    if (ambientGField) ambientGField->set_text(to_string(ambientG));
    if (ambientBField) ambientBField->set_text(to_string(ambientB));
    if (ambientAField) ambientAField->set_text(to_string(ambientA));
    if (diffuseRField) diffuseRField->set_text(to_string(diffuseR));
    if (diffuseGField) diffuseGField->set_text(to_string(diffuseG));
    if (diffuseBField) diffuseBField->set_text(to_string(diffuseB));
    if (diffuseAField) diffuseAField->set_text(to_string(diffuseA));
    if (specularRField) specularRField->set_text(to_string(specularR));
    if (specularGField) specularGField->set_text(to_string(specularG));
    if (specularBField) specularBField->set_text(to_string(specularB));
    if (specularAField) specularAField->set_text(to_string(specularA));
    if (shininessField) shininessField->set_text(to_string(shininess));
}

void sync_material_from_fields(MaterialProperties& mat) {
    mat.ambient = vec4(ambientR, ambientG, ambientB, ambientA);
    mat.diffuse = vec4(diffuseR, diffuseG, diffuseB, diffuseA);
    mat.specular = vec4(specularR, specularG, specularB, specularA);
    mat.shininess = shininess;
}

void sync_color_to_fields(const vec4& color) {
    colorR = color[0]; colorG = color[1];
    colorB = color[2]; colorA = color[3];
    if (colorRField) colorRField->set_text(to_string(colorR));
    if (colorGField) colorGField->set_text(to_string(colorG));
    if (colorBField) colorBField->set_text(to_string(colorB));
    if (colorAField) colorAField->set_text(to_string(colorA));
}

void sync_color_from_fields(vec4& color) {
    color = vec4(colorR, colorG, colorB, colorA);
}

void sync_transform_to_fields(vec3 translate, vec3 rotate, vec3 scale) {
    transX = translate[0]; transY = translate[1]; transZ = translate[2];
    rotX = rotate[0]; rotY = rotate[1]; rotZ = rotate[2];
    scaleX = scale[0]; scaleY = scale[1]; scaleZ = scale[2];

    if (transXField) transXField->set_text(to_string(transX));
    if (transYField) transYField->set_text(to_string(transY));
    if (transZField) transZField->set_text(to_string(transZ));

    if (rotXField) rotXField->set_text(to_string(rotX));
    if (rotYField) rotYField->set_text(to_string(rotY));
    if (rotZField) rotZField->set_text(to_string(rotZ));

    if (scaleXField) scaleXField->set_text(to_string(scaleX));
    if (scaleYField) scaleYField->set_text(to_string(scaleY));
    if (scaleZField) scaleZField->set_text(to_string(scaleZ));
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
        objectNameField->set_text("NONE");
        objectNameField->set_editable(false);
        typeDropdown->set_enabled(false);
        modelDropdown->set_enabled(false);
        selectedType = -1;
        selectedTexture = -1;
        selectedMesh = -1;
        transformSection->hide();
        set_section_visibility(static_cast<DrawType>(-1));
        return;
    }

    if (selectedObject->mesh == -1) {
        typeDropdown->set_enabled(false);
    } else {
        typeDropdown->set_enabled(true);
    }

    objectNameField->set_editable(true);
    nameString = selectedObject->name;
    objectNameField->set_text(nameString);

    modelDropdown->set_enabled(true);
    selectedMesh = (int)selectedObject->mesh;

    selectedType = selectedObject->drawType;
    set_section_visibility(selectedObject->drawType);

    transformSection->show();
    sync_transform_to_fields(selectedObject->position, selectedObject->rotation, selectedObject->scale);

    if (textureDropdown && selectedObject->drawType == TEXTURE) {
        textureDropdown->set_options(textureNames);
        selectedTexture = find_texture_index(selectedObject->texture);
    } else {
        selectedTexture = -1;
    }

    if (selectedObject->drawType == MATERIAL) {
        sync_material_to_fields(dynamicMaterials[selectedObject->material]);
    } else if (selectedObject->drawType == SOLID_COLOR) {
        sync_color_to_fields(selectedObject->color);
    }
}

void update_from_fields() {
    if (!selectedObject) return;

    sync_transform_from_fields({transX, transY, transZ}, {rotX, rotY, rotZ}, {scaleX, scaleY, scaleZ});

    if (selectedMesh >= 0 && selectedMesh < (int)meshNames.size() && selectedObject->mesh != selectedMesh) {
        selectedObject->mesh = selectedMesh;
        if (selectedObject->drawType == SOLID_COLOR && selectedObject->hasColor) {
            build_solid_color_buffer(numVertices[selectedObject->mesh],
                                     selectedObject->color,
                                     selectedObject->colorBuffer);
        }
    }

    if (selectedObject->mesh == -1) {
        typeDropdown->set_enabled(false);
    } else {
        typeDropdown->set_enabled(true);
    }

    if (objectNameField && selectedObject->name != nameString) {
        selectedObject->name = nameString;
        rebuild_scene_graph();
    }

    if (selectedType >= 0 && selectedObject->drawType != static_cast<DrawType>(selectedType)) {
        selectedObject->initialize_type(static_cast<DrawType>(selectedType));
        selectedObject->drawType = static_cast<DrawType>(selectedType);
        update_fields();
        return;
    }

    if (selectedObject->drawType == MATERIAL) {
        sync_material_from_fields(dynamicMaterials[selectedObject->material]);
    } else if (selectedObject->drawType == SOLID_COLOR) {
        vec4 c;
        sync_color_from_fields(c);
        selectedObject->set_color(c);
    } else if (selectedObject->drawType == TEXTURE) {
        if (selectedTexture >= 0 && selectedTexture < (int)TextureIDs.size())
            selectedObject->texture = TextureIDs[selectedTexture];
    }
}

void select_object(SceneObject* obj) {
    selectedObject = obj;
    update_fields();
    rebuild_scene_graph();
}

void delete_selected_object() {
    if (!selectedObject) return;

    auto it = std::find(objects.begin(), objects.end(), selectedObject);
    if (it != objects.end())
        objects.erase(it);

    delete selectedObject;
    selectedObject = nullptr;

    update_fields();
    rebuild_scene_graph();
}

void rebuild_scene_graph() {
    if (!sceneGraphPanel) return;

    for (auto& cell : sceneGraphPanel->cells) {
        if (cell.widget)
            destroy_widget(cell.widget);
    }
    sceneGraphPanel->cells.clear();

    for (int i = 0; i < (int)objects.size(); ++i) {
        SceneObject* obj = objects[i];
        Button* b = new Button(0, 0, 240.0f, 36.0f,
            [obj]() { select_object(obj); },
            obj->name);

        if (obj == selectedObject) {
            b->color = HUD_ACCENT;
        }

        sceneGraphPanel->add(b, 0, i);
    }
}

void show_save_dialog() {
    saveNameString = "untitled";
    if (saveNameField) {
        saveNameField->set_text(saveNameString);
        saveNameField->focused = true;
    }
    if (saveDialogPanel) {
        saveDialogPanel->show();
        saveDialogPanel->x = (ww - saveDialogPanel->w) * 0.5f;
        saveDialogPanel->y = (hh - saveDialogPanel->h) * 0.5f;
    }
}

//-----------------------------------Button Functions--------------------------------------

void toggle_lights() {
    lightOn[WhitePointLight] = !lightOn[WhitePointLight];
}

void add_texture() {
    textureFileBrowser->open_browser();
}

void add_model() {
    meshFileBrowser->open_browser();
}

void add_object() {
    string name = "object" + std::to_string(objects.size() + 1);
    SceneObject* obj = new SceneObject(name);
    select_object(obj);
    rebuild_scene_graph();
    update_fields();
}

void load_scene() {
    sceneFileBrowser->open_browser();
}

void save_scene() {
    show_save_dialog();
}

void hide_save_dialog() {
    if (saveDialogPanel) saveDialogPanel->hide();
    if (saveNameField) saveNameField->focused = false;
}

void confirm_save_scene() {
    string name = saveNameField ? saveNameField->get_text() : saveNameString;

    if (name.empty() || name.find_first_not_of(" \t") == string::npos)
        name = "untitled";

    save_scene_file(name);
    hide_save_dialog();
}

void toggle_camera_mode() {
    if (cameraMode == ORTHO) {
        orthoEye = eye;
        orthoCenter = center;
        orthoAzimuth = azimuth;
        orthoElevation = elevation;
        orthoRadius = radius;
        cameraMode = PERSPECTIVE;
        cameraModeButton->set_label("PERSPECTIVE");
        eye = fpsEye;
    } else {
        fpsEye = eye;
        cameraMode = ORTHO;
        cameraModeButton->set_label("ORTHO");
        eye = orthoEye;
        center = orthoCenter;
        azimuth = orthoAzimuth;
        elevation = orthoElevation;
        radius = orthoRadius;
    }
}