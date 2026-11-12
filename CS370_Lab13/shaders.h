#include "../common/shaderutils.h"

// Shader variables
// Texture shader program references
GLuint textureProgram;
GLuint textureVPos;
GLuint textureVTex;
GLuint textureProjMatLoc;
GLuint textureCameraMatLoc;
GLuint textureModelMatLoc;

// Texture shader files
const char *textureVertexShader = "../texture.vert";
const char *textureFragShader = "../texture.frag";

void build_shaders() {
    // Load texture shaders
    ShaderInfo texture_shaders[] = { {GL_VERTEX_SHADER, textureVertexShader},{GL_FRAGMENT_SHADER, textureFragShader},{GL_NONE, NULL} };
    textureProgram = LoadShaders(texture_shaders);
    textureVPos = glGetAttribLocation(textureProgram, "vPosition");
    textureVTex = glGetAttribLocation(textureProgram, "vTexCoord");
    textureProjMatLoc = glGetUniformLocation(textureProgram, "proj_matrix");
    textureCameraMatLoc = glGetUniformLocation(textureProgram, "camera_matrix");
    textureModelMatLoc = glGetUniformLocation(textureProgram, "model_matrix");
}