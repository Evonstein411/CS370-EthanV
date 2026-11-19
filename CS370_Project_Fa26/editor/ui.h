#ifndef CS370_FA26_EDITORUI_H
#define CS370_FA26_EDITORUI_H

#include "../scene/scenemodel.h"
#include "../nodes/meshcomponent.h"

#include <string>
#include "../kitUI/hud.h"

class EditorUI {
public:
    explicit EditorUI(SceneModel& model);
    ~EditorUI();

    void init();
    void update();      // pull from model → fields
    void render();

    // commands (called by buttons or EditorController)
    void select_node(SceneNode* node);
    void clear_selection();
    void add_object();
    void delete_selected();
    void rebuild_scene_graph();

    void show_save_dialog();
    void confirm_save();
    void hide_save_dialog();

    void toggle_camera_mode();
    void toggle_lights();

private:
    SceneModel& model;

    // top level
    Canvas* root = nullptr;
    Panel* topbar = nullptr;
    Panel* sceneGraphPanel = nullptr;
    Panel* propertiesPanel = nullptr;
    Panel* saveDialog = nullptr;

    // top bar
    Button* addObjectButton = nullptr;
    Button* loadSceneButton = nullptr;
    Button* saveSceneButton = nullptr;
    Button* cameraModeButton = nullptr;
    Button* lightButton = nullptr;

    // scene graph
    Section* sceneGraphSection = nullptr;

    // properties – always visible
    TextField<std::string>* nameField = nullptr;
    Dropdown* meshDropdown = nullptr;

    // appearance mode
    Dropdown* drawModeDropdown = nullptr;

    // material section
    Section* materialSection = nullptr;
    TextField<float>* ambientR = nullptr;
    TextField<float>* ambientG = nullptr;
    TextField<float>* ambientB = nullptr;
    TextField<float>* ambientA = nullptr;
    TextField<float>* diffuseR = nullptr;
    TextField<float>* diffuseG = nullptr;
    TextField<float>* diffuseB = nullptr;
    TextField<float>* diffuseA = nullptr;
    TextField<float>* specularR = nullptr;
    TextField<float>* specularG = nullptr;
    TextField<float>* specularB = nullptr;
    TextField<float>* specularA = nullptr;
    TextField<float>* shininessField = nullptr;

    // solid color section
    Section* colorSection = nullptr;
    TextField<float>* colorR = nullptr;
    TextField<float>* colorG = nullptr;
    TextField<float>* colorB = nullptr;
    TextField<float>* colorA = nullptr;

    // texture section
    Section* textureSection = nullptr;
    Dropdown* textureDropdown = nullptr;
    Button* addTextureButton = nullptr;

    // transform section
    Section* transformSection = nullptr;
    TextField<float>* posX = nullptr;
    TextField<float>* posY = nullptr;
    TextField<float>* posZ = nullptr;
    TextField<float>* rotX = nullptr;
    TextField<float>* rotY = nullptr;
    TextField<float>* rotZ = nullptr;
    TextField<float>* scaleX = nullptr;
    TextField<float>* scaleY = nullptr;
    TextField<float>* scaleZ = nullptr;

    // file browsers
    FileBrowser* sceneBrowser = nullptr;
    FileBrowser* meshBrowser = nullptr;
    FileBrowser* textureBrowser = nullptr;

    // save dialog
    TextField<std::string>* saveNameField = nullptr;
    Button* confirmSaveButton = nullptr;
    Button* cancelSaveButton = nullptr;

    // internal
    void sync_to_fields(SceneNode* node);
    void sync_from_fields(SceneNode* node);

    void show_mesh_properties(const MeshComponent* mesh);
    void hide_mesh_properties();

    void highlight_scene_graph_selection();
    void on_draw_mode_changed(int index);
};

#endif // CS370_FA26_EDITORUI_H