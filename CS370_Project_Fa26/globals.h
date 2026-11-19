#ifndef GLOBALS_H
#define GLOBALS_H

#include "../common/vmath.h"
#include "../common/GLFWutils.h"

using namespace vmath;
using namespace std;





// Global object variables
inline GLfloat sphereAngle = 0.0;
inline GLdouble elTime = 0.0;
inline GLdouble rpm = 10.0;
inline vec3 axis = {0.0f, 1.0f, 0.0f};
inline GLboolean animate = true;
inline GLboolean dirty = false;

inline vec4 backgroundColor = {0.05f, 0.05f, 0.05f, 1.0f};



#endif //GLOBALS_H

