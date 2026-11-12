#include "../common/shaderutils.h"

// Shader variables
// Light shader program references
GLuint lightingProgram;
GLuint lightingVPos;
GLuint lightingVNorm;
GLuint lightingCameraMatLoc;
GLuint lightingModelMatLoc;
GLuint lightingProjMatLoc;
GLuint lightingNormMatLoc;
GLuint lightingLightsBlockIdx;
GLuint lightingMaterialsBlockIdx;
GLuint lightingMaterialLoc;
GLuint lightingNumLightsLoc;
GLuint lightingLightOnLoc;
GLuint lightingEyeLoc;

// Texture shader program references
GLuint textureProgram;
GLuint textureVPos;
GLuint textureVTex;
GLuint textureProjMatLoc;
GLuint textureCameraMatLoc;
GLuint textureModelMatLoc;

// Lighting shader files
const char *lightingVertexShader = "../../common/shaders/Phonglighting.vert";
const char *lightingFragShader = "../../common/shaders/Phonglighting.frag";

// Texture shader files
const char *textureVertexShader = "../../common/shaders/texture.vert";
const char *textureFragShader = "../../common/shaders/texture.frag";

void build_shaders() {
    // Load light shader
    ShaderInfo lighting_shaders[] = { {GL_VERTEX_SHADER, lightingVertexShader},{GL_FRAGMENT_SHADER, lightingFragShader},{GL_NONE, NULL} };
    lightingProgram = LoadShaders(lighting_shaders);
    lightingVPos = glGetAttribLocation(lightingProgram, "vPosition");
    lightingVNorm = glGetAttribLocation(lightingProgram, "vNormal");
    lightingProjMatLoc = glGetUniformLocation(lightingProgram, "proj_matrix");
    lightingCameraMatLoc = glGetUniformLocation(lightingProgram, "camera_matrix");
    lightingNormMatLoc = glGetUniformLocation(lightingProgram, "normal_matrix");
    lightingModelMatLoc = glGetUniformLocation(lightingProgram, "model_matrix");
    lightingLightsBlockIdx = glGetUniformBlockIndex(lightingProgram, "LightBuffer");
    lightingMaterialsBlockIdx = glGetUniformBlockIndex(lightingProgram, "MaterialBuffer");
    lightingMaterialLoc = glGetUniformLocation(lightingProgram, "Material");
    lightingNumLightsLoc = glGetUniformLocation(lightingProgram, "NumLights");
    lightingLightOnLoc = glGetUniformLocation(lightingProgram, "LightOn");
    lightingEyeLoc = glGetUniformLocation(lightingProgram, "EyePosition");

    // Load texture shaders
    ShaderInfo texture_shaders[] = { {GL_VERTEX_SHADER, textureVertexShader},{GL_FRAGMENT_SHADER, textureFragShader},{GL_NONE, NULL} };
    textureProgram = LoadShaders(texture_shaders);
    textureVPos = glGetAttribLocation(textureProgram, "vPosition");
    textureVTex = glGetAttribLocation(textureProgram, "vTexCoord");
    textureProjMatLoc = glGetUniformLocation(textureProgram, "proj_matrix");
    textureCameraMatLoc = glGetUniformLocation(textureProgram, "camera_matrix");
    textureModelMatLoc = glGetUniformLocation(textureProgram, "model_matrix");
}