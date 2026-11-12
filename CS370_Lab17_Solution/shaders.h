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

// Multi-texture shader program references
GLuint multi_tex_program;
GLuint multi_tex_vPos;
GLuint multi_tex_vTex;
GLuint multi_tex_proj_mat_loc;
GLuint multi_tex_camera_mat_loc;
GLuint multi_tex_model_mat_loc;
GLuint multi_tex_base_loc;
GLuint multi_tex_blend_loc;
GLuint multi_tex_mix_loc;

// Lighting shader files
const char *lightingVertexShader = "../../common/shaders/Phonglighting.vert";
const char *lightingFragShader = "../../common/shaders/Phonglighting.frag";

// Multitexture shader files
const char *multi_tex_vertex_shader = "../multiTex.vert";
const char *multi_tex_frag_shader = "../multiTex.frag";

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

    // Load multi texture shader
    ShaderInfo multi_tex_shaders[] = { {GL_VERTEX_SHADER, multi_tex_vertex_shader},{GL_FRAGMENT_SHADER, multi_tex_frag_shader},{GL_NONE, NULL} };
    multi_tex_program = LoadShaders(multi_tex_shaders);
    multi_tex_vPos = glGetAttribLocation(multi_tex_program, "vPosition");
    multi_tex_vTex = glGetAttribLocation(multi_tex_program, "vTexCoord");
    multi_tex_proj_mat_loc = glGetUniformLocation(multi_tex_program, "proj_matrix");
    multi_tex_camera_mat_loc = glGetUniformLocation(multi_tex_program, "camera_matrix");
    multi_tex_model_mat_loc = glGetUniformLocation(multi_tex_program, "model_matrix");
    multi_tex_base_loc = glGetUniformLocation(multi_tex_program, "baseMap");
    // TODO: Associate references with shader sampler and mix variables
    multi_tex_blend_loc = glGetUniformLocation(multi_tex_program, "blendMap");
    multi_tex_mix_loc = glGetUniformLocation(multi_tex_program, "mixFactor");
}