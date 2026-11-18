#ifndef GLOBALS_H
#define GLOBALS_H

#include "../common/vmath.h"
#include "../common/GLFWutils.h"

using namespace vmath;
using namespace std;

// Buffer enums
enum ObjBuffer_IDs {PosBuffer, NormBuffer, TexBuffer, TangBuffer, BiTangBuffer, NumObjBuffers};
enum LightBuffer_IDs {LightBuffer, NumLightBuffers};
enum MaterialBuffer_IDs {MaterialBuffer, NumMaterialBuffers};

// Number of component coordinates
inline GLint posCoords = 4;
inline GLint normCoords = 3;
inline GLint texCoords = 2;
inline GLint colCoords = 4;

// Global state
inline mat4 projMatrix;
inline mat4 cameraMatrix;
inline mat4 normalMatrix;
inline mat4 modelMatrix;

// Initial Camera
enum CameraMode {ORTHO, PERSPECTIVE};

inline CameraMode cameraMode = ORTHO;

//Global camera state
inline vec3 eye = {3.0f, 3.0f, 3.0f};
inline vec3 center = {0.0f, 0.0f, 0.0f};
inline vec3 up = {0.0f, 1.0f, 0.0f};
inline GLfloat azimuth = 320.0f;
inline GLfloat elevation = 60.0f;
inline GLfloat radius = 17.5f;
inline GLfloat del = 2.0f;
inline GLfloat daz = 2.0f;
inline GLfloat dr = 0.1f;
inline GLfloat minRadius = 0.5f;

//Ortho camera
inline vec3 orthoEye = {3.0f, 3.0f, 3.0f};
inline vec3 orthoCenter = {0.0f, 0.0f, 0.0f};
inline GLfloat orthoAzimuth = 320.0f;
inline GLfloat orthoElevation = 60.0f;
inline GLfloat orthoRadius = 17.5f;

//Perspective (first person) camera
inline vec3 fpsEye = {0.0f, 2.0f, 10.0f};
inline GLfloat fpsAzimuth = 0.0f;
inline GLfloat fpsElevation = 0.0f;
inline GLfloat fpsStep = 0.25f;
inline GLfloat fpsTurn = 2.0f;

// Global object variables
inline GLfloat sphereAngle = 0.0;
inline GLdouble elTime = 0.0;
inline GLdouble rpm = 10.0;
inline vec3 axis = {0.0f, 1.0f, 0.0f};
inline GLboolean animate = true;
inline GLboolean dirty = false;

inline vec4 backgroundColor = {0.05f, 0.05f, 0.05f, 1.0f};

// Global screen dimensions
inline GLint ww = 1640;
inline GLint hh = 960;

#endif //GLOBALS_H

