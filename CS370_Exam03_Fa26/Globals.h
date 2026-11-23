#include "Exam03.h"
#define NUM_MODES 10

using namespace vmath;
using namespace std;

// Buffer enums
enum ObjBuffer_IDs {PosBuffer, NormBuffer, TexBuffer, NumObjBuffers};
enum LightBuffer_IDs {LightBuffer, NumLightBuffers};
enum MaterialBuffer_IDs {MaterialBuffer, NumMaterialBuffers};

// Number of component coordinates
GLint posCoords = 4;
GLint normCoords = 3;
GLint texCoords = 2;
GLint colCoords = 4;

// Global state
mat4 proj_matrix;
mat4 camera_matrix;
mat4 normal_matrix;
mat4 model_matrix;

// Initial Camera
vec3 eye = {4.0f, 4.0f, 4.0f};
vec3 center = {0.0f, 0.0f, 0.0f};
vec3 up = {0.0f, 1.0f, 0.0f};

// Global spherical coord values
GLfloat azimuth = -45.0f;
GLfloat daz = 2.0f;
GLfloat elevation = 65.0f;
GLfloat del = 2.0f;
GLfloat radius = 1.4641f;

// Global object variables
GLdouble flashTime = 0.0;
GLdouble elTime = 0.0;
GLboolean animate = false;
GLint mode = 0;

// Global screen dimensions
GLint ww = 640;
GLint hh = 480;
