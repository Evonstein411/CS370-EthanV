#include "Exam02.h"
#define NUM_MODES 9

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
vec3 eye = {1.0f, 1.0f, 1.0f};
vec3 center = {0.0f, 0.0f, 0.0f};
vec3 up = {0.0f, 1.0f, 0.0f};

// Global spherical camera variables
GLfloat azimuth = 45.0f;
GLfloat daz = 2.0f;
GLfloat elevation = 54.7f;
GLfloat del = 2.0f;
GLfloat radius = 1.732f;

// Global object variables
GLfloat spm = 10;
GLfloat plate_theta = 0.0;
GLboolean spinning = false;
GLfloat ppm = 6.0;
GLfloat pot_theta = 0.0f;
GLboolean tipping = true;
GLint pour_dir = -1.0;
GLfloat pour_ang = -30.0f;
GLboolean pouring = false;
GLfloat coffee_y = 0.0;
GLfloat coffee_fill = 1.0f;
GLdouble elTime = 0.0;
GLint mode = 0;

// Global screen dimensions
GLint ww = 640;
GLint hh = 480;
