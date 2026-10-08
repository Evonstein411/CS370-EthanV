// Shader variables
// Light shader program with shadows reference
extern GLuint phong_shadow_program;
extern GLuint phong_shadow_vPos;
extern GLuint phong_shadow_vNorm;
extern GLuint phong_shadow_proj_mat_loc;
extern GLuint phong_shadow_camera_mat_loc;
extern GLuint phong_shadow_norm_mat_loc;
extern GLuint phong_shadow_model_mat_loc;
extern GLuint phong_shadow_shad_proj_mat_loc;
extern GLuint phong_shadow_shad_cam_mat_loc;
extern GLuint phong_shadow_lights_block_idx;
extern GLuint phong_shadow_materials_block_idx;
extern GLuint phong_shadow_material_loc;
extern GLuint phong_shadow_num_lights_loc;
extern GLuint phong_shadow_light_on_loc;
extern GLuint phong_shadow_eye_loc;

// Shadow map shader program reference
extern GLuint shadow_program;
extern GLuint shadow_vPos;
extern GLuint shadow_proj_mat_loc;
extern GLuint shadow_camera_mat_loc;
extern GLuint shadow_model_mat_loc;

// Debug shadow program reference
extern GLuint debug_program;
