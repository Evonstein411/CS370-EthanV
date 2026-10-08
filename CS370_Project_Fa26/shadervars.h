// Shader variables
// Color shader program references
extern GLuint color_program;
extern GLuint color_vPos;
extern GLuint color_vCol;
extern GLuint color_proj_mat_loc;
extern GLuint color_cam_mat_loc;
extern GLuint color_model_mat_loc;

// Light shader program references
extern GLuint lighting_program;
extern GLuint lighting_vPos;
extern GLuint lighting_vNorm;
extern GLuint lighting_camera_mat_loc;
extern GLuint lighting_model_mat_loc;
extern GLuint lighting_proj_mat_loc;
extern GLuint lighting_norm_mat_loc;
extern GLuint lighting_lights_block_idx;
extern GLuint lighting_materials_block_idx;
extern GLuint lighting_material_loc;
extern GLuint lighting_num_lights_loc;
extern GLuint lighting_light_on_loc;
extern GLuint lighting_eye_loc;

// Texture shader program references
extern GLuint texture_program;
extern GLuint texture_vPos;
extern GLuint texture_vTex;
extern GLuint texture_proj_mat_loc;
extern GLuint texture_camera_mat_loc;
extern GLuint texture_model_mat_loc;
extern GLuint texture_uv_scale_loc;
extern GLuint texture_uv_offset_loc;