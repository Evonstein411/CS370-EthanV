#include "../common/shaderutils.h"

// Shader variables
// Color shader program references
GLuint color_program;
GLuint color_vPos;
GLuint color_vCol;
GLuint color_proj_mat_loc;
GLuint color_cam_mat_loc;
GLuint color_model_mat_loc;

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

// Texture shader program references
GLuint texture_program;
GLuint texture_vPos;
GLuint texture_vTex;
GLuint texture_proj_mat_loc;
GLuint texture_camera_mat_loc;
GLuint texture_model_mat_loc;
GLuint texture_uv_scale_loc;
GLuint texture_uv_offset_loc;

// Color shader files
const char *color_vertex_shader = "../../common/shaders/color.vert";
const char *color_frag_shader = "../../common/shaders/color.frag";

// Lighting shader files
const char *lighting_vertex_shader = "../../common/shaders/Phonglighting.vert";
const char *lighting_frag_shader = "../../common/shaders/Phonglighting.frag";

// Texture shader files
const char *texture_vertex_shader = "../../CS370_Project_Fa26/shaders/texture2.vert";
const char *texture_frag_shader = "../../CS370_Project_Fa26/shaders/texture2.frag";


void build_shaders() {
     // Load color shader
	ShaderInfo color_shaders[] = { {GL_VERTEX_SHADER, color_vertex_shader},{GL_FRAGMENT_SHADER, color_frag_shader},{GL_NONE, NULL} };
	color_program = LoadShaders(color_shaders);
    color_vPos = glGetAttribLocation(color_program, "vPosition");
    color_vCol = glGetAttribLocation(color_program, "vColor");
    color_proj_mat_loc = glGetUniformLocation(color_program, "proj_matrix");
    color_cam_mat_loc = glGetUniformLocation(color_program, "camera_matrix");
    color_model_mat_loc = glGetUniformLocation(color_program, "model_matrix");

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
    ShaderInfo texture_shaders[] = { {GL_VERTEX_SHADER, texture_vertex_shader},{GL_FRAGMENT_SHADER, texture_frag_shader},{GL_NONE, NULL} };
    texture_program = LoadShaders(texture_shaders);
    texture_vPos = glGetAttribLocation(texture_program, "vPosition");
    texture_vTex = glGetAttribLocation(texture_program, "vTexCoord");
    texture_proj_mat_loc = glGetUniformLocation(texture_program, "proj_matrix");
    texture_camera_mat_loc = glGetUniformLocation(texture_program, "camera_matrix");
    texture_model_mat_loc = glGetUniformLocation(texture_program, "model_matrix");
	texture_uv_scale_loc = glGetUniformLocation(texture_program, "uvScale");
	texture_uv_offset_loc = glGetUniformLocation(texture_program, "uvOffset");

	glUseProgram(texture_program);
	glUniform1i(glGetUniformLocation(texture_program, "tex"), 0);

}