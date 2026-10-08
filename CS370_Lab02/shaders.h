#include "../common/shaderutils.h"

// Shader variables
// Color shader program references
GLuint color_program;
GLuint color_vPos;
GLuint color_vCol;
GLuint color_proj_mat_loc;
GLuint color_cam_mat_loc;
GLuint color_model_mat_loc;

// Color shader files
const char *color_vertex_shader = "../../common/shaders/color.vert";
const char *color_frag_shader = "../../common/shaders/color.frag";

void build_shaders() {
    // Load color shader
	ShaderInfo color_shaders[] = { {GL_VERTEX_SHADER, color_vertex_shader},{GL_FRAGMENT_SHADER, color_frag_shader},{GL_NONE, NULL} };
	color_program = LoadShaders(color_shaders);
    color_vPos = glGetAttribLocation(color_program, "vPosition");
    color_vCol = glGetAttribLocation(color_program, "vColor");
    color_proj_mat_loc = glGetUniformLocation(color_program, "proj_matrix");
    color_cam_mat_loc = glGetUniformLocation(color_program, "camera_matrix");
    color_model_mat_loc = glGetUniformLocation(color_program, "model_matrix");
}