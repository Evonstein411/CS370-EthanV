#include "../common/shaderutils.h"

// Shader variables
// Color shader program references
GLuint color_program;
GLuint color_vPos;
GLuint color_vCol;
GLuint color_proj_mat_loc;
GLuint color_cam_mat_loc;
GLuint color_model_mat_loc;

// Dim shader program references
GLuint dim_program;
GLuint dim_vPos;
GLuint dim_vCol;
GLuint dim_proj_mat_loc;
GLuint dim_cam_mat_loc;
GLuint dim_model_mat_loc;
GLuint dim_factor_loc;

// Color shader files
const char *color_vertex_shader = "../../common/shaders/color.vert";
const char *color_frag_shader = "../../common/shaders/color.frag";
const char *dim_vertex_shader = "../dim.vert";

void build_shaders() {
    // Load color shader
	ShaderInfo color_shaders[] = { {GL_VERTEX_SHADER, color_vertex_shader},{GL_FRAGMENT_SHADER, color_frag_shader},{GL_NONE, NULL} };
	// TODO: Build color shader program
	color_program = LoadShaders(color_shaders);
	// TODO: Get attribute shader variable references
    color_vPos = glGetAttribLocation(color_program, "vPosition");
    color_vCol = glGetAttribLocation(color_program, "vColor");
	// TODO: Get uniform shader variable references
    color_proj_mat_loc = glGetUniformLocation(color_program, "proj_matrix");
    color_cam_mat_loc = glGetUniformLocation(color_program, "camera_matrix");
    color_model_mat_loc = glGetUniformLocation(color_program, "model_matrix");

    // Load dimmed shader
    ShaderInfo dim_shaders[] = { {GL_VERTEX_SHADER, dim_vertex_shader},{GL_FRAGMENT_SHADER, color_frag_shader},{GL_NONE, NULL} };
	// TODO: Build dimmed shader program
    dim_program = LoadShaders(dim_shaders);
	// TODO: Get attribute shader variable references
    dim_vPos = glGetAttribLocation(dim_program, "vPosition");
    dim_vCol = glGetAttribLocation(dim_program, "vColor");
	// TODO: Get uniform shader variable references
    dim_proj_mat_loc = glGetUniformLocation(dim_program, "proj_matrix");
    dim_cam_mat_loc = glGetUniformLocation(dim_program, "camera_matrix");
    dim_model_mat_loc = glGetUniformLocation(dim_program, "model_matrix");
	// TODO: Get uniform shader reference for dim factor
    dim_factor_loc = glGetUniformLocation(dim_program, "dim_factor");
}