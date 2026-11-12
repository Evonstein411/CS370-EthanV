#include "../common/shaderutils.h"

// ---------------------
// Fa26 DO NOT MODIFY!!!
// ---------------------

// Shader variables
// Color shader program references
GLuint colorProgram;
GLuint colorVPos;
GLuint colorVCol;
GLuint colorProjMatLoc;
GLuint colorCamMatLoc;
GLuint colorModelMatLoc;

// Color shader files
const char *colorVertexShader = "../../common/shaders/color.vert";
const char *colorFragShader = "../../common/shaders/color.frag";

void build_shaders() {
    // Load color shader
	ShaderInfo color_shaders[] = { {GL_VERTEX_SHADER, colorVertexShader},{GL_FRAGMENT_SHADER, colorFragShader},{GL_NONE, NULL} };
	colorProgram = LoadShaders(color_shaders);
    colorVPos = glGetAttribLocation(colorProgram, "vPosition");
    colorVCol = glGetAttribLocation(colorProgram, "vColor");
    colorProjMatLoc = glGetUniformLocation(colorProgram, "proj_matrix");
    colorCamMatLoc = glGetUniformLocation(colorProgram, "camera_matrix");
    colorModelMatLoc = glGetUniformLocation(colorProgram, "model_matrix");
}