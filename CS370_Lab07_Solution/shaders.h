#include "../common/shaderutils.h"

// Shader variables
// Color shader program references
GLuint colorProgram;
GLuint colorVPos;
GLuint colorVCol;
GLuint colorProjMatLoc;
GLuint colorCamMatLoc;
GLuint colorModelMatLoc;

// Dim shader program references
GLuint dim_program;
GLuint dim_vPos;
GLuint dim_vCol;
GLuint dim_proj_mat_loc;
GLuint dim_cam_mat_loc;
GLuint dim_model_mat_loc;
GLuint dim_factor_loc;

// Color shader files
const char *colorVertexShader = "../../common/shaders/color.vert";
const char *colorFragShader = "../../common/shaders/color.frag";
const char *dim_vertex_shader = "../dim.vert";

void build_shaders() {
    // Load color shader
	ShaderInfo color_shaders[] = { {GL_VERTEX_SHADER, colorVertexShader},{GL_FRAGMENT_SHADER, colorFragShader},{GL_NONE, NULL} };
	// TODO: Build color shader program
	colorProgram = LoadShaders(color_shaders);
	// TODO: Get attribute shader variable references
    colorVPos = glGetAttribLocation(colorProgram, "vPosition");
    colorVCol = glGetAttribLocation(colorProgram, "vColor");
	// TODO: Get uniform shader variable references
    colorProjMatLoc = glGetUniformLocation(colorProgram, "proj_matrix");
    colorCamMatLoc = glGetUniformLocation(colorProgram, "camera_matrix");
    colorModelMatLoc = glGetUniformLocation(colorProgram, "model_matrix");

    // Load dimmed shader
    ShaderInfo dim_shaders[] = { {GL_VERTEX_SHADER, dim_vertex_shader},{GL_FRAGMENT_SHADER, colorFragShader},{GL_NONE, NULL} };
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