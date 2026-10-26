
//Header file for all widget declarations


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
