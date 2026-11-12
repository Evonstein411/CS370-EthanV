#include "../common/shaderutils.h"

// Shader variables
// Color shader program references
GLuint colorProgram;
GLuint colorVPos;
GLuint colorVCol;
GLuint colorProjMatLoc;
GLuint colorCamMatLoc;
GLuint colorModelMatLoc;

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

// Color shader files
const char *colorVertexShader = "../../common/shaders/color.vert";
const char *colorFragShader = "../../common/shaders/color.frag";

// Lighting shader files
const char *lightingVertexShader = "../../common/shaders/Phonglighting.vert";
const char *lightingFragShader = "../../common/shaders/Phonglighting.frag";

void build_shaders() {
    // Load color shader
	ShaderInfo color_shaders[] = { {GL_VERTEX_SHADER, colorVertexShader},{GL_FRAGMENT_SHADER, colorFragShader},{GL_NONE, NULL} };
	colorProgram = LoadShaders(color_shaders);
    colorVPos = glGetAttribLocation(colorProgram, "vPosition");
    colorVCol = glGetAttribLocation(colorProgram, "vColor");
    colorProjMatLoc = glGetUniformLocation(colorProgram, "proj_matrix");
    colorCamMatLoc = glGetUniformLocation(colorProgram, "camera_matrix");
    colorModelMatLoc = glGetUniformLocation(colorProgram, "model_matrix");

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
}