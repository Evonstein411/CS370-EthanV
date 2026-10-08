// Shader variables
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

// Multi-texture shader program references
extern GLuint multi_tex_program;
extern GLuint multi_tex_vPos;
extern GLuint multi_tex_vTex;
extern GLuint multi_tex_proj_mat_loc;
extern GLuint multi_tex_camera_mat_loc;
extern GLuint multi_tex_model_mat_loc;
extern GLuint multi_tex_base_loc;
extern GLuint multi_tex_blend_loc;
extern GLuint multi_tex_mix_loc;

// Bumpmapping shader program references
extern GLuint bump_program;
extern GLuint bump_proj_mat_loc;
extern GLuint bump_camera_mat_loc;
extern GLuint bump_norm_mat_loc;
extern GLuint bump_model_mat_loc;
extern GLuint bump_vPos;
extern GLuint bump_vNorm;
extern GLuint bump_vTex;
extern GLuint bump_vTang;
extern GLuint bump_vBiTang;
extern GLuint bump_lights_block_idx;
extern GLuint bump_num_lights_loc;
extern GLuint bump_light_on_loc;
extern GLuint bump_eye_loc;
extern GLuint bump_base_loc;
extern GLuint bump_norm_loc;
