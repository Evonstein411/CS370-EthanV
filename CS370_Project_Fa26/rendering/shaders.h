#ifndef SHADERS_H
#define SHADERS_H

#include "../../common/shaderutils.h"

// Shader variables
// Color shader program references
inline GLuint colorProgram;
inline GLuint colorVPos;
inline GLuint colorVCol;
inline GLuint colorProjMatLoc;
inline GLuint colorCamMatLoc;
inline GLuint colorModelMatLoc;

// Light shader program references
inline GLuint lightingProgram;
inline GLuint lightingVPos;
inline GLuint lightingVNorm;
inline GLuint lightingCameraMatLoc;
inline GLuint lightingModelMatLoc;
inline GLuint lightingProjMatLoc;
inline GLuint lightingNormMatLoc;
inline GLuint lightingLightsBlockIdx;
inline GLuint lightingMaterialsBlockIdx;
inline GLuint lightingMaterialLoc;
inline GLuint lightingNumLightsLoc;
inline GLuint lightingLightOnLoc;
inline GLuint lightingEyeLoc;

// Object Texture shader program references
inline GLuint textureProgram;
inline GLuint textureVPos;
inline GLuint textureVTex;
inline GLuint textureProjMatLoc;
inline GLuint textureCameraMatLoc;
inline GLuint textureModelMatLoc;
inline GLuint textureUvScaleLoc;
inline GLuint textureUvOffsetLoc;

//HUD Texture shader program
inline GLuint hudProgram;
inline GLuint hudVPos;
inline GLuint hudVTex;
inline GLuint hudProjMatLoc;
inline GLuint hudCameraMatLoc;
inline GLuint hudModelMatLoc;
inline GLuint hudUvScaleLoc;
inline GLuint hudUvOffsetLoc;

//Mask shader program
inline GLuint maskProgram;
inline GLuint maskVPos;
inline GLuint maskProjMatLoc;
inline GLuint maskCamMatLoc;
inline GLuint maskModelMatLoc;

//Outline shader refs
inline GLuint outlineProgram;
inline GLuint outlineVPos;
inline GLuint outlineVTex;
inline GLuint outlineMaskTexLoc;
inline GLuint outlineTexelSizeLoc;
inline GLuint outlineColorLoc;
inline GLuint outlineThicknessLoc;

// Color shader files
inline auto colorVertexShader = "../../common/shaders/color.vert";
inline auto *colorFragShader = "../../common/shaders/color.frag";

// Lighting shader files
inline auto *lightingVertexShader = "../../common/shaders/Phonglighting.vert";
inline  auto *lightingFragShader = "../../common/shaders/Phonglighting.frag";

// Texture shader files
inline auto  *textureVertexShader = "../../CS370_Project_Fa26/shaders/objectTexture.vert";
inline auto  *textureFragShader = "../../CS370_Project_Fa26/shaders/objectTexture.frag";

//HUD shader files
inline auto  *hudVertexShader = "../../CS370_Project_Fa26/shaders/hudTexture.vert";
inline auto  *hudFragShader = "../../CS370_Project_Fa26/shaders/hudTexture.frag";

//Outline shader files
inline auto  *outlineVertexShader = "../../CS370_Project_Fa26/shaders/outline.vert";
inline auto  *outlineFragShader   = "../../CS370_Project_Fa26/shaders/outline.frag";

//Mask shader files
inline auto  *maskVertexShader = "../../CS370_Project_Fa26/shaders/mask.vert";
inline auto  *maskFragShader   = "../../CS370_Project_Fa26/shaders/mask.frag";


inline void build_shaders() {
     // Load color shader
	ShaderInfo color_shaders[] = { {GL_VERTEX_SHADER, colorVertexShader},{GL_FRAGMENT_SHADER, colorFragShader},{GL_NONE, nullptr} };
	colorProgram = LoadShaders(color_shaders);
    colorVPos = glGetAttribLocation(colorProgram, "vPosition");
    colorVCol = glGetAttribLocation(colorProgram, "vColor");
    colorProjMatLoc = glGetUniformLocation(colorProgram, "proj_matrix");
    colorCamMatLoc = glGetUniformLocation(colorProgram, "camera_matrix");
    colorModelMatLoc = glGetUniformLocation(colorProgram, "model_matrix");

   // Load light shader
    ShaderInfo lighting_shaders[] = { {GL_VERTEX_SHADER, lightingVertexShader},{GL_FRAGMENT_SHADER, lightingFragShader},{GL_NONE, nullptr} };
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
    ShaderInfo texture_shaders[] = { {GL_VERTEX_SHADER, textureVertexShader},{GL_FRAGMENT_SHADER, textureFragShader},{GL_NONE, nullptr} };
    textureProgram = LoadShaders(texture_shaders);
    textureVPos = glGetAttribLocation(textureProgram, "vPosition");
    textureVTex = glGetAttribLocation(textureProgram, "vTexCoord");
    textureProjMatLoc = glGetUniformLocation(textureProgram, "proj_matrix");
    textureCameraMatLoc = glGetUniformLocation(textureProgram, "camera_matrix");
    textureModelMatLoc = glGetUniformLocation(textureProgram, "model_matrix");
	textureUvScaleLoc = glGetUniformLocation(textureProgram, "uvScale");
	textureUvOffsetLoc = glGetUniformLocation(textureProgram, "uvOffset");

	// Load hud texture shaders
	ShaderInfo hud_shaders[] = { {GL_VERTEX_SHADER, hudVertexShader},{GL_FRAGMENT_SHADER, hudFragShader},{GL_NONE, nullptr} };
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
		{GL_NONE, nullptr}
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
		{GL_NONE, nullptr}
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