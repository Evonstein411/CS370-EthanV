#include "editorui.h"
#include "../kitUI/hud.h"
#include "../scene/scene.h"
#include "../nodes/scenenode.h"
#include "../nodes/meshcomponent.h"
#include "../rendering/textures.h"
#include "../rendering/geometry.h"
#include "../rendering/materials.h"
#include "../rendering/lights.h"
#include <cstring>



EditorUI::EditorUI(SceneModel& model) : model(model) {}

EditorUI::~EditorUI() {
}

void EditorUI::init() {
    static const std::vector<std::string> drawModeNames = {
        "None", "Solid Color", "Material", "Texture"
    };

    // top bar
    topbar = new Panel(1640.0f, 64.0f);

    lightButton = new Button(200.0f, 40.0f, "TOGGLE LIGHTS");
    lightButton->on_press = [this] { toggle_lights(); };
    topbar->add(lightButton, 0, 0);

    addObjectButton = new Button(140.0f, 40.0f, "Add Object");
    addObjectButton->on_press = [this] { add_object(); };
    topbar->add(addObjectButton, 1, 0);

    loadSceneButton = new Button(140.0f, 40.0f, "Load Scene");
    loadSceneButton->on_press = [this] { sceneBrowser->open_browser(); };
    topbar->add(loadSceneButton, 2, 0);

    saveSceneButton = new Button(140.0f, 40.0f, "Save Scene");
    saveSceneButton->on_press = [this] { show_save_dialog(); };
    topbar->add(saveSceneButton, 3, 0);

    cameraModeButton = new Button(160.0f, 40.0f, "ORTHO");
    cameraModeButton->on_press = [this] { toggle_camera_mode(); };
    topbar->add(cameraModeButton, 4, 0);

    // scene graph
    sceneGraphSection = new Section(220.0f, 0.0f, "Scene Graph");
    sceneGraphPanel = new Panel(200.0f, 0.0f);
    sceneGraphSection->add(sceneGraphPanel);

    // properties
    propertiesPanel = new Panel(320.0f, 0.0f);

    auto* objectSection = new Section(300.0f, 0.0f, "Object");
    propertiesPanel->add(objectSection, 0, 0);
    auto* settings = new Panel(280.0f, 0.0f);
    objectSection->add(settings);

    nameProperty = new StringProperty(settings, 0, 0, "Name:");

    meshDropdown = new Dropdown(220.0f, 32.0f, "Mesh:", meshNames, &selectedMesh);
    settings->add(meshDropdown, 0, 1);

    auto* addModelBtn = new Button(140.0f, 32.0f, "Add Model");
    addModelBtn->on_press = [this] { meshBrowser->open_browser(); };
    settings->add(addModelBtn, 1, 1);

    drawModeDropdown = new Dropdown(220.0f, 32.0f, "Draw Type:", drawModeNames, &selectedType);
    settings->add(drawModeDropdown, 0, 2);

    // material
    materialSection = new Section(300.0f, 0.0f, "Material");
    propertiesPanel->add(materialSection, 0, 1);
    auto* matPanel = new Panel(280.0f, 0.0f);
    materialSection->add(matPanel);

    ambientProperty   = new Vec4Property(matPanel, 0, "ambient");
    diffuseProperty   = new Vec4Property(matPanel, 1, "diffuse");
    specularProperty  = new Vec4Property(matPanel, 2, "specular");
    shininessProperty = new FloatProperty(matPanel, 1, 3, "");

    // solid color
    colorSection = new Section(300.0f, 0.0f, "Solid Color");
    propertiesPanel->add(colorSection, 0, 2);
    auto* colorPanel = new Panel(280.0f, 0.0f);
    colorSection->add(colorPanel);
    colorProperty = new Vec4Property(colorPanel, 0, "color");

    // texture
    textureSection = new Section(300.0f, 0.0f, "Texture");
    propertiesPanel->add(textureSection, 0, 3);
    auto* texPanel = new Panel(280.0f, 0.0f);
    textureSection->add(texPanel);

    textureDropdown = new Dropdown(200.0f, 32.0f, "Texture:", textureNames, &selectedTexture);
    texPanel->add(textureDropdown, 0, 0);

    addTextureButton = new Button(140.0f, 32.0f, "Add Texture");
    addTextureButton->on_press = [this] { textureBrowser->open_browser(); };
    texPanel->add(addTextureButton, 1, 0);

    // transform
    transformSection = new Section(300.0f, 0.0f, "Transform");
    propertiesPanel->add(transformSection, 0, 4);
    auto* xformPanel = new Panel(280.0f, 0.0f);
    transformSection->add(xformPanel);

    posProperty   = new Vec3Property(xformPanel, 0, "translation");
    rotProperty   = new Vec3Property(xformPanel, 1, "rotation");
    scaleProperty = new Vec3Property(xformPanel, 2, "scale");

    // file browsers
    sceneBrowser = new FileBrowser();
    sceneBrowser->set_filter({".df"});
    sceneBrowser->set_start_location(FileBrowser::StartLocation::CWD);
    sceneBrowser->onFileSelected = [this](const std::string& path) {
        if (Scene* s = model.get_active_scene())
            s->load(path);
        rebuild_scene_graph();
    };

    meshBrowser = new FileBrowser();
    meshBrowser->set_filter({".obj"});
    meshBrowser->onFileSelected = [this](const std::string& path) {
        int index = add_model_file(path);
        if (index < 0) return;
        selectedMesh = index;
        if (meshDropdown) meshDropdown->set_options(meshNames);
        // apply to selected mesh if any
    };

    textureBrowser = new FileBrowser();
    textureBrowser->set_filter({".png", ".jpg", ".jpeg"});
    textureBrowser->onFileSelected = [this](const std::string& path) {
        int texIndex = add_texture_file(path);
        if (texIndex < 0) return;
        selectedTexture = texIndex;
        if (textureDropdown) textureDropdown->set_options(textureNames);
    };

    // save dialog
    saveDialog = new Panel(280.0f, 140.0f);
    saveDialog->set_priority(10);
    auto* saveLabel = new Label(200.0f, 28.0f, "Save Scene As:");
    saveDialog->add(saveLabel, 0, 0, 2, 1);
    saveNameProperty = new StringProperty(saveDialog, 0, 1, "Name:");
    confirmSaveButton = new Button(100.0f, 32.0f, "Save");
    confirmSaveButton->on_press = [this] { confirm_save(); };
    saveDialog->add(confirmSaveButton, 0, 2);
    cancelSaveButton = new Button(100.0f, 32.0f, "Cancel");
    cancelSaveButton->on_press = [this] { hide_save_dialog(); };
    saveDialog->add(cancelSaveButton, 1, 2);
    saveDialog->hide();

    rebuild_scene_graph();
    update();
}

void EditorUI::update() {
    sync_to_fields(model.get_selected());
}

void EditorUI::apply() {
    if (auto* node = model.get_selected())
        sync_from_fields(node);
}

void EditorUI::sync_to_fields(SceneNode* node) {
    if (!node) {
        if (nameProperty) nameProperty->set("");
        hide_mesh_properties();
        return;
    }

    nameProperty->set(node->name);

    if (auto* mesh = node->get_component<MeshComponent>()) {
        // transform lives on the node
        posProperty->set(node->position);
        rotProperty->set(node->rotation);
        scaleProperty->set(node->scale);
        transformSection->show();

        selectedMesh = static_cast<int>(mesh->mesh);
        selectedDrawMode = static_cast<int>(mesh->appearance.mode);

        switch (mesh->appearance.mode) {
        case DrawMode::Material:
            ambientProperty->set(mesh->appearance.material.ambient);
            diffuseProperty->set(mesh->appearance.material.diffuse);
            specularProperty->set(mesh->appearance.material.specular);
            shininessProperty->set(mesh->appearance.material.shininess);
            materialSection->show();
            colorSection->hide();
            textureSection->hide();
            break;

        case DrawMode::SolidColor:
            colorProperty->set(mesh->appearance.color);
            colorSection->show();
            materialSection->hide();
            textureSection->hide();
            break;

        case DrawMode::Texture:
            // optional: look up index from mesh->appearance.texture
            textureSection->show();
            materialSection->hide();
            colorSection->hide();
            break;

        default:
            hide_mesh_properties();
            break;
        }
    } else {
        hide_mesh_properties();
    }
}

void EditorUI::sync_from_fields(SceneNode* node) {
    if (!node) return;

    node->name = nameProperty->get();

    if (auto* mesh = node->get_component<MeshComponent>()) {
        node->position = posProperty->get();
        node->rotation = rotProperty->get();
        node->scale    = scaleProperty->get();

        mesh->appearance.mode = static_cast<DrawMode>(selectedDrawMode);

        switch (mesh->appearance.mode) {
        case DrawMode::Material:
            mesh->appearance.material.ambient   = ambientProperty->get();
            mesh->appearance.material.diffuse   = diffuseProperty->get();
            mesh->appearance.material.specular  = specularProperty->get();
            mesh->appearance.material.shininess = shininessProperty->get();
            break;

        case DrawMode::SolidColor:
            mesh->appearance.color = colorProperty->get();
            break;

        case DrawMode::Texture:
            if (selectedTexture >= 0 && selectedTexture < (int)TextureIDs.size())
                mesh->appearance.texture = TextureIDs[selectedTexture];
            break;

        default:
            break;
        }

        mesh->appearance.build(mesh->mesh);
    }
}

void EditorUI::select_node(SceneNode* node) {
    model.set_selected(node);
    update();
    // highlight scene graph if desired
}

void EditorUI::clear_selection() {
    model.set_selected(nullptr);
    update();
}

void EditorUI::add_object() {
    Scene* scene = model.get_active_scene();
    if (!scene) return;

    std::string name = "object" + std::to_string(scene->roots.size() + 1);
    auto* node = new SceneNode(name);
    node->set_component(std::make_unique<MeshComponent>());
    scene->add_root(node);
    rebuild_scene_graph();
    select_node(node);
}

void EditorUI::delete_selected() {
    SceneNode* sel = model.get_selected();
    if (!sel) return;
    if (Scene* scene = model.get_active_scene())
        scene->remove_root(sel);
    model.clear_selection();
    update();
    rebuild_scene_graph();
}

void EditorUI::rebuild_scene_graph() {
    if (!sceneGraphPanel) return;

    for (auto& cell : sceneGraphPanel->cells)
        delete cell.widget;
    sceneGraphPanel->cells.clear();
    sceneGraphPanel->children.clear();

    Scene* scene = model.get_active_scene();
    if (!scene) return;

    int row = 0;
    scene->for_each([&](SceneNode* n) {
        if (!n) return;
        auto* b = new Button(200.0f, 32.0f, n->name);
        b->on_press = [this, n] { select_node(n); };
        sceneGraphPanel->add(b, 0, row++);
    });
}

void EditorUI::show_save_dialog() {
    if (saveNameProperty) saveNameProperty->set("untitled");
    if (saveDialog) {
        saveDialog->show();
        // center using HUD screen size
        float sw = static_cast<float>(HUD::instance().get_screen_width());
        float sh = static_cast<float>(HUD::instance().get_screen_height());
        saveDialog->x = (sw - saveDialog->w) * 0.5f;
        saveDialog->y = (sh - saveDialog->h) * 0.5f;
    }
}

void EditorUI::hide_save_dialog() {
    if (saveDialog) saveDialog->hide();
}

void EditorUI::confirm_save() {
    std::string name = saveNameProperty ? saveNameProperty->get() : "untitled";
    if (name.empty() || name.find_first_not_of(" \t") == std::string::npos)
        name = "untitled";
    if (Scene* s = model.get_active_scene())
        s->save(name);
    hide_save_dialog();
}

void EditorUI::toggle_camera_mode() {
    model.toggle_camera_mode();
    if (cameraModeButton) {
        bool ortho = model.get_active_camera_mode() == CameraMode::ORTHO;
        cameraModeButton->set_label(ortho ? "ORTHO" : "PERSPECTIVE");
    }
}

void EditorUI::toggle_lights() {
    lightOn[WhitePointLight] = !lightOn[WhitePointLight];
}

void EditorUI::show_mesh_properties(const MeshComponent* /*mesh*/) {
    // optional: extra visibility logic
}

void EditorUI::hide_mesh_properties() {
    if (materialSection) materialSection->hide();
    if (colorSection) colorSection->hide();
    if (textureSection) textureSection->hide();
    if (transformSection) transformSection->hide();
}

void EditorUI::on_draw_mode_changed(int /*index*/) {
    // called when drawModeDropdown changes; update section visibility
    update();
}