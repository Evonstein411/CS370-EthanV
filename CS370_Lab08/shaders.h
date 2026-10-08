#include "../common/shaderutils.h"

// Shader variables
// Fog shader program references
GLuint fog_program;
GLuint fog_vPos;
GLuint fog_vCol;
GLuint fog_proj_mat_loc;
GLuint fog_cam_mat_loc;
GLuint fog_model_mat_loc;
GLuint fog_fog_col_loc;
GLuint fog_fog_start_loc;
GLuint fog_fog_end_loc;

// Lighting shader files
const char *color_vertex_shader = "../color.vert";
const char *fog_frag_shader = "../fog.frag";

void build_shaders() {
    // Load fog shader
    ShaderInfo fog_shaders[] = { {GL_VERTEX_SHADER, color_vertex_shader},{GL_FRAGMENT_SHADER, fog_frag_shader},{GL_NONE, NULL} };
    fog_program = LoadShaders(fog_shaders);
    fog_vPos = glGetAttribLocation(fog_program, "vPosition");
    fog_vCol = glGetAttribLocation(fog_program, "vColor");
    fog_proj_mat_loc = glGetUniformLocation(fog_program, "proj_matrix");
    fog_cam_mat_loc = glGetUniformLocation(fog_program, "camera_matrix");
    fog_model_mat_loc = glGetUniformLocation(fog_program, "model_matrix");
    fog_fog_col_loc = glGetUniformLocation(fog_program, "fog_color");
    fog_fog_start_loc = glGetUniformLocation(fog_program, "fog_start");
    fog_fog_end_loc = glGetUniformLocation(fog_program, "fog_end");
}