#ifndef SHADERS_H
#define SHADERS_H

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

// Object Texture shader program references
GLuint textureProgram;
GLuint textureVPos;
GLuint textureVTex;
GLuint textureProjMatLoc;
GLuint textureCameraMatLoc;
GLuint textureModelMatLoc;
GLuint textureUvScaleLoc;
GLuint textureUvOffsetLoc;

//HUD Texture shader program
GLuint hudProgram;
GLuint hudVPos;
GLuint hudVTex;
GLuint hudProjMatLoc;
GLuint hudCameraMatLoc;
GLuint hudModelMatLoc;
GLuint hudUvScaleLoc;
GLuint hudUvOffsetLoc;

//Mask shader program
GLuint maskProgram;
GLuint maskVPos;
GLuint maskProjMatLoc;
GLuint maskCamMatLoc;
GLuint maskModelMatLoc;

//Outline shader refs
GLuint outlineProgram;
GLuint outlineVPos;
GLuint outlineVTex;
GLuint outlineMaskTexLoc;
GLuint outlineTexelSizeLoc;
GLuint outlineColorLoc;
GLuint outlineThicknessLoc;

// Color shader files
const char *colorVertexShader = "../../common/shaders/color.vert";
const char *colorFragShader = "../../common/shaders/color.frag";

// Lighting shader files
const char *lightingVertexShader = "../../common/shaders/Phonglighting.vert";
const char *lightingFragShader = "../../common/shaders/Phonglighting.frag";

// Texture shader files
const char *textureVertexShader = "../../CS370_Project_Fa26/shaders/objectTexture.vert";
const char *textureFragShader = "../../CS370_Project_Fa26/shaders/objectTexture.frag";

//HUD shader files
const char *hudVertexShader = "../../CS370_Project_Fa26/shaders/hudTexture.vert";
const char *hudFragShader = "../../CS370_Project_Fa26/shaders/hudTexture.frag";

//Outline shader files
const char *outlineVertexShader = "../../CS370_Project_Fa26/shaders/outline.vert";
const char *outlineFragShader   = "../../CS370_Project_Fa26/shaders/outline.frag";

//Mask shader files
const char *maskVertexShader = "../../CS370_Project_Fa26/shaders/mask.vert";
const char *maskFragShader   = "../../CS370_Project_Fa26/shaders/mask.frag";


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

    // Load object texture shaders
    ShaderInfo texture_shaders[] = { {GL_VERTEX_SHADER, textureVertexShader},{GL_FRAGMENT_SHADER, textureFragShader},{GL_NONE, NULL} };
    textureProgram = LoadShaders(texture_shaders);
    textureVPos = glGetAttribLocation(textureProgram, "vPosition");
    textureVTex = glGetAttribLocation(textureProgram, "vTexCoord");
    textureProjMatLoc = glGetUniformLocation(textureProgram, "proj_matrix");
    textureCameraMatLoc = glGetUniformLocation(textureProgram, "camera_matrix");
    textureModelMatLoc = glGetUniformLocation(textureProgram, "model_matrix");
	textureUvScaleLoc = glGetUniformLocation(textureProgram, "uvScale");
	textureUvOffsetLoc = glGetUniformLocation(textureProgram, "uvOffset");

	// Load hud texture shaders
	ShaderInfo hud_shaders[] = { {GL_VERTEX_SHADER, hudVertexShader},{GL_FRAGMENT_SHADER, hudFragShader},{GL_NONE, NULL} };
	hudProgram = LoadShaders(hud_shaders);
	hudVPos = glGetAttribLocation(hudProgram, "vPosition");
	hudVTex = glGetAttribLocation(hudProgram, "vTexCoord");
	hudProjMatLoc = glGetUniformLocation(hudProgram, "proj_matrix");
	hudCameraMatLoc = glGetUniformLocation(hudProgram, "camera_matrix");
	hudModelMatLoc = glGetUniformLocation(hudProgram, "model_matrix");
	hudUvScaleLoc = glGetUniformLocation(hudProgram, "uvScale");
	hudUvOffsetLoc = glGetUniformLocation(hudProgram, "uvOffset");

	// Load mask shaders
	ShaderInfo mask_shaders[] = {
		{GL_VERTEX_SHADER, maskVertexShader},
		{GL_FRAGMENT_SHADER, maskFragShader},
		{GL_NONE, NULL}
	};
	maskProgram = LoadShaders(mask_shaders);
	maskVPos         = glGetAttribLocation(maskProgram, "vPosition");
	maskProjMatLoc = glGetUniformLocation(maskProgram, "proj_matrix");
	maskCamMatLoc  = glGetUniformLocation(maskProgram, "camera_matrix");
	maskModelMatLoc= glGetUniformLocation(maskProgram, "model_matrix");

	// Load outline (edge) shaders
	ShaderInfo outline_shaders[] = {
		{GL_VERTEX_SHADER, outlineVertexShader},
		{GL_FRAGMENT_SHADER, outlineFragShader},
		{GL_NONE, NULL}
	};
	outlineProgram = LoadShaders(outline_shaders);
	outlineVPos            = glGetAttribLocation(outlineProgram, "vPosition");
	outlineVTex            = glGetAttribLocation(outlineProgram, "vTexCoord");
	outlineMaskTexLoc    = glGetUniformLocation(outlineProgram, "maskTex");
	outlineTexelSizeLoc  = glGetUniformLocation(outlineProgram, "texelSize");
	outlineColorLoc       = glGetUniformLocation(outlineProgram, "outlineColor");
	outlineThicknessLoc   = glGetUniformLocation(outlineProgram, "thickness");

	// Set texture unit for the mask sampler
	glUseProgram(outlineProgram);
	glUniform1i(outlineMaskTexLoc, 0);

	glUseProgram(hudProgram);
	glUniform1i(glGetUniformLocation(hudProgram, "tex"), 0);

	glUseProgram(textureProgram);
	glUniform1i(glGetUniformLocation(textureProgram, "tex"), 0);

}

#endif //SHADERS_H