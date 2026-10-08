#include "../common/shaderutils.h"

// Shader variables
// Light shader program with shadows reference
GLuint phong_shadow_program;
GLuint phong_shadow_vPos;
GLuint phong_shadow_vNorm;
GLuint phong_shadow_proj_mat_loc;
GLuint phong_shadow_camera_mat_loc;
GLuint phong_shadow_norm_mat_loc;
GLuint phong_shadow_model_mat_loc;
GLuint phong_shadow_shad_proj_mat_loc;
GLuint phong_shadow_shad_cam_mat_loc;
GLuint phong_shadow_lights_block_idx;
GLuint phong_shadow_materials_block_idx;
GLuint phong_shadow_material_loc;
GLuint phong_shadow_num_lights_loc;
GLuint phong_shadow_light_on_loc;
GLuint phong_shadow_eye_loc;

// Shadow map shader program reference
GLuint shadow_program;
GLuint shadow_vPos;
GLuint shadow_proj_mat_loc;
GLuint shadow_camera_mat_loc;
GLuint shadow_model_mat_loc;

// Debug shadow program reference
GLuint debug_program;

// Lighting shader with shadows files
const char *phong_shadow_vertex_shader = "../phongShadow.vert";
const char *phong_shadow_frag_shader = "../phongShadow.frag";

// Shadow map shader files
const char *shadow_vertex_shader = "../../common/shaders/shadow.vert";
const char *shadow_frag_shader = "../../common/shaders/shadow.frag";

// Debug shadow shader files
const char *debug_shadow_vertex_shader = "../../common/shaders/debugShadow.vert";
const char *debug_shadow_frag_shader = "../../common/shaders/debugShadow.frag";


void build_shaders() {
    // Load light shader with shadows
	ShaderInfo phong_shadow_shaders[] = { {GL_VERTEX_SHADER, phong_shadow_vertex_shader},{GL_FRAGMENT_SHADER, phong_shadow_frag_shader},{GL_NONE, NULL} };
    phong_shadow_program = LoadShaders(phong_shadow_shaders);
    phong_shadow_vPos = glGetAttribLocation(phong_shadow_program, "vPosition");
    phong_shadow_vNorm = glGetAttribLocation(phong_shadow_program, "vNormal");
    phong_shadow_camera_mat_loc = glGetUniformLocation(phong_shadow_program, "camera_matrix");
    phong_shadow_proj_mat_loc = glGetUniformLocation(phong_shadow_program, "proj_matrix");
    phong_shadow_norm_mat_loc = glGetUniformLocation(phong_shadow_program, "normal_matrix");
    phong_shadow_model_mat_loc = glGetUniformLocation(phong_shadow_program, "model_matrix");
    phong_shadow_shad_proj_mat_loc = glGetUniformLocation(phong_shadow_program, "light_proj_matrix");
    phong_shadow_shad_cam_mat_loc = glGetUniformLocation(phong_shadow_program, "light_cam_matrix");
    phong_shadow_lights_block_idx = glGetUniformBlockIndex(phong_shadow_program, "LightBuffer");
    phong_shadow_materials_block_idx = glGetUniformBlockIndex(phong_shadow_program, "MaterialBuffer");
    phong_shadow_material_loc = glGetUniformLocation(phong_shadow_program, "Material");
    phong_shadow_num_lights_loc = glGetUniformLocation(phong_shadow_program, "NumLights");
    phong_shadow_light_on_loc = glGetUniformLocation(phong_shadow_program, "LightOn");
    phong_shadow_eye_loc = glGetUniformLocation(phong_shadow_program, "EyePosition");

    // Load shadow map shader
    ShaderInfo shadow_shaders[] = { {GL_VERTEX_SHADER, shadow_vertex_shader},{GL_FRAGMENT_SHADER, shadow_frag_shader},{GL_NONE, NULL} };
    shadow_program = LoadShaders(shadow_shaders);
    shadow_vPos = glGetAttribLocation(shadow_program, "vPosition");
    shadow_proj_mat_loc = glGetUniformLocation(shadow_program, "light_proj_matrix");
    shadow_camera_mat_loc = glGetUniformLocation(shadow_program, "light_cam_matrix");
    shadow_model_mat_loc = glGetUniformLocation(shadow_program, "model_matrix");
    
    // Load shadow debug shader
    ShaderInfo debug_shaders[] = { {GL_VERTEX_SHADER, debug_shadow_vertex_shader},{GL_FRAGMENT_SHADER, debug_shadow_frag_shader},{GL_NONE, NULL} };
    debug_program = LoadShaders(debug_shaders);

}