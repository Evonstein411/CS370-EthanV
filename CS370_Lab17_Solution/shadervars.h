// Shader variables
// Light shader program references
extern GLuint lightingProgram;
extern GLuint lightingVPos;
extern GLuint lightingVNorm;
extern GLuint lightingCameraMatLoc;
extern GLuint lightingModelMatLoc;
extern GLuint lightingProjMatLoc;
extern GLuint lightingNormMatLoc;
extern GLuint lightingLightsBlockIdx;
extern GLuint lightingMaterialsBlockIdx;
extern GLuint lightingMaterialLoc;
extern GLuint lightingNumLightsLoc;
extern GLuint lightingLightOnLoc;
extern GLuint lightingEyeLoc;

// Multi-texture shader program references
extern GLuint multi_tex_program;
extern GLuint multi_tex_vPos;
extern GLuint multi_tex_vTex;
extern GLuint multi_tex_proj_mat_loc;
extern GLuint multi_tex_camera_mat_loc;
extern GLuint multi_tex_model_mat_loc;
extern GLuint multi_tex_base_loc;
extern GLuint multi_tex_blend_loc;
extern GLuint multi_tex_mix_loc;
