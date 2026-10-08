#include "TrainConsts.h"
// Component indices
#define X 0
#define Y 1
#define Z 2

using namespace vmath;
using namespace std;

// Buffer enums
enum ObjBuffer_IDs {PosBuffer, NormBuffer, TexBuffer, NumObjBuffers};

// Number of component coordinates
GLint posCoords = 4;
GLint normCoords = 3;
GLint texCoords = 2;
GLint colCoords = 4;

// Global state
mat4 proj_matrix;
mat4 camera_matrix;
mat4 model_matrix;

// Initial Camera
vec3 eye = {3.0f, 3.0f, 3.0f};
vec3 center = {0.0f, 0.0f, 0.0f};
vec3 up = {0.0f, 1.0f, 0.0f};

// View modes
#define ORTHOGRAPHIC 0
#define PERSPECTIVE 1
int proj = ORTHOGRAPHIC;

// Global spherical coord values
GLfloat azimuth = 45.0f;
//GLfloat azimuth = 90.0f;
GLfloat daz = 1.0f;
GLfloat elevation = 53.5f;
//GLfloat elevation = 90.0f;
GLfloat del = 1.0f;
GLfloat radius = 7.5f;
GLfloat dr = 0.1f;
GLfloat min_radius = 2.0f;

// Global object variables
GLdouble speed = (RAIL_LENGTH/4.0f);
GLdouble rpm = 50.0;
GLdouble smokeSpeed = (RAIL_LENGTH/8.0f);
GLfloat smokeOffset = 0.0f;
vec3 train_pos = {0.0f, 0.0f, 0.0f};
GLint train_dir = -1;
GLfloat wheel_ang = 0.0f;
GLdouble elTime = 0.0;
GLboolean animate = false;

// Global screen dimensions
GLint ww = 640;
GLint hh = 480;

std::vector<vec3> treePositions;