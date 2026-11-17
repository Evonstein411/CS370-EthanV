#pragma once
#include "../kitUI/hud.h"
#include "../scene/nodes.h"
#include "../common/vmath.h"          // vec3, vec4
#include "../common/GLFWutils.h" //gluint
#include "../common/lighting.h" //material properties
#include <string>
#include <vector>




// globals
extern int selectedMesh;
extern int selectedType;
extern int selectedTexture;
extern std::string nameString;
inline const std::vector<std::string> drawTypeNames = {
    "None",
    "Solid Color",
    "Material",
    "Texture"
};

// file browsers
extern FileBrowser* textureFileBrowser;
extern FileBrowser* meshFileBrowser;
extern FileBrowser* sceneFileBrowser;

// sections
extern Section* materialSection;
extern Section* objectSection;
extern Section* solidColorSection;
extern Section* textureSection;
extern Section* transformSection;
extern Section* sceneGraphSection;

// canvas
extern Canvas* topbar;

// panels
extern Panel* topbarPanel;
extern Panel* objectPanel;
extern Panel* settingsPanel;
extern Panel* materialPanel;
extern Panel* solidColorPanel;
extern Panel* texturePanel;
extern Panel* transformPanel;
extern Panel* saveDialogPanel;
extern Panel* sceneGraphPanel;

// dropdowns
extern Dropdown* typeDropdown;
extern Dropdown* modelDropdown;
extern Dropdown* textureDropdown;

// labels
extern Label* ambientLabel;
extern Label* diffuseLabel;
extern Label* specularLabel;
extern Label* shininessLabel;
extern Label* translationLabel;
extern Label* rotationLabel;
extern Label* scaleLabel;
extern Label* saveSceneLabel;

// buttons
extern Button* lightButton;
extern Button* addTextureButton;
extern Button* addModelButton;
extern Button* addObjectButton;
extern Button* loadSceneButton;
extern Button* saveSceneButton;
extern Button* saveNameButton;
extern Button* cancelSaveButton;
extern Button* cameraModeButton;

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
extern TextField<std::string>* objectNameField;
extern TextField<std::string>* saveNameField;

extern TextField<float>* ambientRField;
extern TextField<float>* ambientGField;
extern TextField<float>* ambientBField;
extern TextField<float>* ambientAField;
extern TextField<float>* diffuseRField;
extern TextField<float>* diffuseGField;
extern TextField<float>* diffuseBField;
extern TextField<float>* diffuseAField;
extern TextField<float>* specularRField;
extern TextField<float>* specularGField;
extern TextField<float>* specularBField;
extern TextField<float>* specularAField;
extern TextField<float>* shininessField;
extern TextField<float>* colorRField;
extern TextField<float>* colorGField;
extern TextField<float>* colorBField;
extern TextField<float>* colorAField;
extern TextField<float>* transXField;
extern TextField<float>* transYField;
extern TextField<float>* transZField;
extern TextField<float>* rotXField;
extern TextField<float>* rotYField;
extern TextField<float>* rotZField;
extern TextField<float>* scaleXField;
extern TextField<float>* scaleYField;
extern TextField<float>* scaleZField;

// field variables
extern float transX, transY, transZ;
extern float rotX, rotY, rotZ;
extern float scaleX, scaleY, scaleZ;
extern std::string saveNameString;

//functions
void update_fields();
void update_from_fields();
void rebuild_scene_graph();
void select_object(SceneNode* node);
MeshNode* selected_mesh();
void render_hud();
void init_hud();
void set_section_visibility(const char* typeName);
void sync_material_to_fields(const MaterialProperties& mat);
void sync_material_from_fields(MaterialProperties& mat);
void sync_color_to_fields(const vmath::vec4& color);
void sync_color_from_fields(vmath::vec4& color);
void sync_transform_to_fields(vmath::vec3 translate, vmath::vec3 rotate, vmath::vec3 scale);
void sync_transform_from_fields(vmath::vec3 t, vmath::vec3 r, vmath::vec3 s);
int find_texture_index(GLuint texID);
void highlight_scene_graph_selection();
void delete_selected_object();
void show_save_dialog();
void add_vec4_fields(Panel* panel, int row,
                     Label*& label, const char* labelText,
                     TextField<float>*& r, TextField<float>*& g, TextField<float>*& b, TextField<float>*& a,
                     float& rVal, float& gVal, float& bVal, float& aVal);
