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



// Global object variables
inline GLfloat sphereAngle = 0.0;
inline GLdouble elTime = 0.0;
inline GLdouble rpm = 10.0;
inline vec3 axis = {0.0f, 1.0f, 0.0f};
inline GLboolean animate = true;
inline GLboolean dirty = false;

inline vec4 backgroundColor = {0.05f, 0.05f, 0.05f, 1.0f};



#endif //GLOBALS_H

