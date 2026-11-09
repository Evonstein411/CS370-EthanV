#ifndef SHADERS_H
#define SHADERS_H

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

// Object Texture shader program references
GLuint texture_program;
GLuint texture_vPos;
GLuint texture_vTex;
GLuint texture_proj_mat_loc;
GLuint texture_camera_mat_loc;
GLuint texture_model_mat_loc;
GLuint texture_uv_scale_loc;
GLuint texture_uv_offset_loc;

//HUD Texture shader program
GLuint hud_program;
GLuint hud_vPos;
GLuint hud_vTex;
GLuint hud_proj_mat_loc;
GLuint hud_camera_mat_loc;
GLuint hud_model_mat_loc;
GLuint hud_uv_scale_loc;
GLuint hud_uv_offset_loc;

//Mask shader program
GLuint mask_program;
GLuint mask_vPos;
GLuint mask_proj_mat_loc;
GLuint mask_cam_mat_loc;
GLuint mask_model_mat_loc;

//Outline shader refs
GLuint outline_program;
GLuint outline_vPos;
GLuint outline_vTex;
GLuint outline_mask_tex_loc;
GLuint outline_texel_size_loc;
GLuint outline_color_loc;
GLuint outline_thickness_loc;

// Color shader files
const char *color_vertex_shader = "../../common/shaders/color.vert";
const char *color_frag_shader = "../../common/shaders/color.frag";

// Lighting shader files
const char *lighting_vertex_shader = "../../common/shaders/Phonglighting.vert";
const char *lighting_frag_shader = "../../common/shaders/Phonglighting.frag";

// Texture shader files
const char *texture_vertex_shader = "../../CS370_Project_Fa26/shaders/objectTexture.vert";
const char *texture_frag_shader = "../../CS370_Project_Fa26/shaders/objectTexture.frag";

//HUD shader files
const char *hud_vertex_shader = "../../CS370_Project_Fa26/shaders/hudTexture.vert";
const char *hud_frag_shader = "../../CS370_Project_Fa26/shaders/hudTexture.frag";

//Outline shader files
const char *outline_vertex_shader = "../../CS370_Project_Fa26/shaders/outline.vert";
const char *outline_frag_shader   = "../../CS370_Project_Fa26/shaders/outline.frag";

//Mask shader files
const char *mask_vertex_shader = "../../CS370_Project_Fa26/shaders/mask.vert";
const char *mask_frag_shader   = "../../CS370_Project_Fa26/shaders/mask.frag";


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

    // Load object texture shaders
    ShaderInfo texture_shaders[] = { {GL_VERTEX_SHADER, texture_vertex_shader},{GL_FRAGMENT_SHADER, texture_frag_shader},{GL_NONE, NULL} };
    texture_program = LoadShaders(texture_shaders);
    texture_vPos = glGetAttribLocation(texture_program, "vPosition");
    texture_vTex = glGetAttribLocation(texture_program, "vTexCoord");
    texture_proj_mat_loc = glGetUniformLocation(texture_program, "proj_matrix");
    texture_camera_mat_loc = glGetUniformLocation(texture_program, "camera_matrix");
    texture_model_mat_loc = glGetUniformLocation(texture_program, "model_matrix");
	texture_uv_scale_loc = glGetUniformLocation(texture_program, "uvScale");
	texture_uv_offset_loc = glGetUniformLocation(texture_program, "uvOffset");

	// Load hud texture shaders
	ShaderInfo hud_shaders[] = { {GL_VERTEX_SHADER, hud_vertex_shader},{GL_FRAGMENT_SHADER, hud_frag_shader},{GL_NONE, NULL} };
	hud_program = LoadShaders(hud_shaders);
	hud_vPos = glGetAttribLocation(hud_program, "vPosition");
	hud_vTex = glGetAttribLocation(hud_program, "vTexCoord");
	hud_proj_mat_loc = glGetUniformLocation(hud_program, "proj_matrix");
	hud_camera_mat_loc = glGetUniformLocation(hud_program, "camera_matrix");
	hud_model_mat_loc = glGetUniformLocation(hud_program, "model_matrix");
	hud_uv_scale_loc = glGetUniformLocation(hud_program, "uvScale");
	hud_uv_offset_loc = glGetUniformLocation(hud_program, "uvOffset");

	// Load mask shaders
	ShaderInfo mask_shaders[] = {
		{GL_VERTEX_SHADER, mask_vertex_shader},
		{GL_FRAGMENT_SHADER, mask_frag_shader},
		{GL_NONE, NULL}
	};
	mask_program = LoadShaders(mask_shaders);
	mask_vPos         = glGetAttribLocation(mask_program, "vPosition");
	mask_proj_mat_loc = glGetUniformLocation(mask_program, "proj_matrix");
	mask_cam_mat_loc  = glGetUniformLocation(mask_program, "camera_matrix");
	mask_model_mat_loc= glGetUniformLocation(mask_program, "model_matrix");

	// Load outline (edge) shaders
	ShaderInfo outline_shaders[] = {
		{GL_VERTEX_SHADER, outline_vertex_shader},
		{GL_FRAGMENT_SHADER, outline_frag_shader},
		{GL_NONE, NULL}
	};
	outline_program = LoadShaders(outline_shaders);
	outline_vPos            = glGetAttribLocation(outline_program, "vPosition");
	outline_vTex            = glGetAttribLocation(outline_program, "vTexCoord");
	outline_mask_tex_loc    = glGetUniformLocation(outline_program, "maskTex");
	outline_texel_size_loc  = glGetUniformLocation(outline_program, "texelSize");
	outline_color_loc       = glGetUniformLocation(outline_program, "outlineColor");
	outline_thickness_loc   = glGetUniformLocation(outline_program, "thickness");

	// Set texture unit for the mask sampler
	glUseProgram(outline_program);
	glUniform1i(outline_mask_tex_loc, 0);

	glUseProgram(hud_program);
	glUniform1i(glGetUniformLocation(hud_program, "tex"), 0);

	glUseProgram(texture_program);
	glUniform1i(glGetUniformLocation(texture_program, "tex"), 0);

}

#endif //SHADERS_H