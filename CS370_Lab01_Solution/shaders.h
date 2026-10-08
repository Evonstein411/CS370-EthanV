#include "../common/shaderutils.h"

// Shader variables
// Basic shader program references
GLuint basic_program;
GLuint basic_vPos;

// Basic shader source files
const char *basic_vertex_shader = "../basic.vert";
const char *basic_frag_shader = "../basic.frag";

void build_shaders() {
    // Load basic shader
	ShaderInfo basic_shaders[] = { {GL_VERTEX_SHADER, basic_vertex_shader},{GL_FRAGMENT_SHADER, basic_frag_shader},{GL_NONE, NULL} };
	basic_program = LoadShaders(basic_shaders);
	basic_vPos = glGetAttribLocation(basic_program, "vPosition");
}