#ifndef CS370_FA26_EDITORUI_H
#define CS370_FA26_EDITORUI_H

#include "../scene/scenemodel.h"
#include "../nodes/meshcomponent.h"
#include <string>
#include "../kitUI/hud.h"
#include "../kitUI/widgets.h"
#include "property.h"


class EditorUI {
public:
    explicit EditorUI(SceneModel& model);
    ~EditorUI();

    void init();
    void update();      // pull from model → fields
    void apply();       // fields → model

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
    int selectedMesh = -1;
    int selectedType = -1;
    int selectedTexture = -1;
    int selectedDrawMode = 0;

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

    Section* sceneGraphSection = nullptr;

    // properties
    StringProperty* nameProperty = nullptr;
    Dropdown* meshDropdown = nullptr;
    Dropdown* drawModeDropdown = nullptr;

    Section* materialSection = nullptr;
    Vec4Property* ambientProperty = nullptr;
    Vec4Property* diffuseProperty = nullptr;
    Vec4Property* specularProperty = nullptr;
    FloatProperty* shininessProperty = nullptr;

    Section* colorSection = nullptr;
    Vec4Property* colorProperty = nullptr;

    Section* textureSection = nullptr;
    Dropdown* textureDropdown = nullptr;
    Button* addTextureButton = nullptr;

    Section* transformSection = nullptr;
    Vec3Property* posProperty = nullptr;
    Vec3Property* rotProperty = nullptr;
    Vec3Property* scaleProperty = nullptr;

    FileBrowser* sceneBrowser = nullptr;
    FileBrowser* meshBrowser = nullptr;
    FileBrowser* textureBrowser = nullptr;

    StringProperty* saveNameProperty = nullptr;
    Button* confirmSaveButton = nullptr;
    Button* cancelSaveButton = nullptr;

    void sync_to_fields(SceneNode* node);
    void sync_from_fields(SceneNode* node);
    void show_mesh_properties(const MeshComponent* mesh);
    void hide_mesh_properties();
    void on_draw_mode_changed(int index);


};

#endif // CS370_FA26_EDITORUI_H