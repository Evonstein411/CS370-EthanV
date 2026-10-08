#include "../common/shaderutils.h"

// Shader variables
// Light shader program references
GLuint lighting_program;
GLuint lighting_vPos;
GLuint lighting_vNorm;
GLuint lighting_camera_mat_loc;
GLuint lighting_model_mat_loc;
GLuint lighting_proj_mat_loc;
GLuint lighting_norm_mat_loc;
GLuint lighting_lights_block_idx;
GLuint lighting_materials_block_idx;
GLuint lighting_material_loc;
GLuint lighting_num_lights_loc;
GLuint lighting_light_on_loc;
GLuint lighting_eye_loc;

// Multi-texture shader program references
GLuint multi_tex_program;
GLuint multi_tex_vPos;
GLuint multi_tex_vTex;
GLuint multi_tex_proj_mat_loc;
GLuint multi_tex_camera_mat_loc;
GLuint multi_tex_model_mat_loc;
GLuint multi_tex_base_loc;
GLuint multi_tex_blend_loc;
GLuint multi_tex_mix_loc;

// Bumpmapping shader program references
GLuint bump_program;
GLuint bump_proj_mat_loc;
GLuint bump_camera_mat_loc;
GLuint bump_norm_mat_loc;
GLuint bump_model_mat_loc;
GLuint bump_vPos;
GLuint bump_vNorm;
GLuint bump_vTex;
GLuint bump_vTang;
GLuint bump_vBiTang;
GLuint bump_lights_block_idx;
GLuint bump_num_lights_loc;
GLuint bump_light_on_loc;
GLuint bump_eye_loc;
GLuint bump_base_loc;
GLuint bump_norm_loc;

// Lighting shader files
const char *lighting_vertex_shader = "../../common/shaders/Phonglighting.vert";
const char *lighting_frag_shader = "../../common/shaders/Phonglighting.frag";

// Multitexture shader files
const char *multi_tex_vertex_shader = "../../common/shaders/multiTex.vert";
const char *multi_tex_frag_shader = "../../common/shaders/multiTex.frag";

// Bumpmapping shader files
const char *bump_vertex_shader = "../bumpTex.vert";
const char *bump_frag_shader = "../bumpTex.frag";

void build_shaders() {
    // Load light shader
    ShaderInfo lighting_shaders[] = { {GL_VERTEX_SHADER, lighting_vertex_shader},{GL_FRAGMENT_SHADER, lighting_frag_shader},{GL_NONE, NULL} };
    lighting_program = LoadShaders(lighting_shaders);
    lighting_vPos = glGetAttribLocation(lighting_program, "vPosition");
    lighting_vNorm = glGetAttribLocation(lighting_program, "vNormal");
    lighting_proj_mat_loc = glGetUniformLocation(lighting_program, "proj_matrix");
    lighting_camera_mat_loc = glGetUniformLocation(lighting_program, "camera_matrix");
    lighting_norm_mat_loc = glGetUniformLocation(lighting_program, "normal_matrix");
    lighting_model_mat_loc = glGetUniformLocation(lighting_program, "model_matrix");
    lighting_lights_block_idx = glGetUniformBlockIndex(lighting_program, "LightBuffer");
    lighting_materials_block_idx = glGetUniformBlockIndex(lighting_program, "MaterialBuffer");
    lighting_material_loc = glGetUniformLocation(lighting_program, "Material");
    lighting_num_lights_loc = glGetUniformLocation(lighting_program, "NumLights");
    lighting_light_on_loc = glGetUniformLocation(lighting_program, "LightOn");
    lighting_eye_loc = glGetUniformLocation(lighting_program, "EyePosition");

    // Load texture shaders
    ShaderInfo multi_tex_shaders[] = { {GL_VERTEX_SHADER, multi_tex_vertex_shader},{GL_FRAGMENT_SHADER, multi_tex_frag_shader},{GL_NONE, NULL} };
    multi_tex_program = LoadShaders(multi_tex_shaders);
    multi_tex_vPos = glGetAttribLocation(multi_tex_program, "vPosition");
    multi_tex_vTex = glGetAttribLocation(multi_tex_program, "vTexCoord");
    multi_tex_proj_mat_loc = glGetUniformLocation(multi_tex_program, "proj_matrix");
    multi_tex_camera_mat_loc = glGetUniformLocation(multi_tex_program, "camera_matrix");
    multi_tex_model_mat_loc = glGetUniformLocation(multi_tex_program, "model_matrix");
    multi_tex_base_loc = glGetUniformLocation(multi_tex_program, "baseMap");
    multi_tex_blend_loc = glGetUniformLocation(multi_tex_program, "blendMap");
    multi_tex_mix_loc = glGetUniformLocation(multi_tex_program, "mixFactor");

    // Load bump shader
    ShaderInfo bump_shaders[] = { {GL_VERTEX_SHADER, bump_vertex_shader},{GL_FRAGMENT_SHADER, bump_frag_shader},{GL_NONE, NULL} };
    bump_program = LoadShaders(bump_shaders);
    bump_vPos = glGetAttribLocation(bump_program, "vPosition");
    bump_vNorm = glGetAttribLocation(bump_program, "vNormal");
    bump_vTex = glGetAttribLocation(bump_program, "vTexCoord");
    bump_vTang = glGetAttribLocation(bump_program, "vTangent");
    bump_vBiTang = glGetAttribLocation(bump_program, "vBiTangent");
    bump_proj_mat_loc = glGetUniformLocation(bump_program, "proj_matrix");
    bump_camera_mat_loc = glGetUniformLocation(bump_program, "camera_matrix");
    bump_norm_mat_loc = glGetUniformLocation(bump_program, "normal_matrix");
    bump_model_mat_loc = glGetUniformLocation(bump_program, "model_matrix");
    bump_lights_block_idx = glGetUniformBlockIndex(bump_program, "LightBuffer");
    bump_num_lights_loc = glGetUniformLocation(bump_program, "NumLights");
    bump_light_on_loc = glGetUniformLocation(bump_program, "LightOn");
    bump_eye_loc = glGetUniformLocation(bump_program, "EyePosition");
    bump_base_loc = glGetUniformLocation(bump_program, "baseMap");
    bump_norm_loc = glGetUniformLocation(bump_program, "normalMap");
}