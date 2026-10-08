// ---------------------
// Fa26 DO NOT MODIFY!!!
// ---------------------

#define NUM_MODES 7
#define TABLE_SIZE 8.0f
#define TARGET_Y 0.0f
#define TARGET_X 1.0f
#define TARGET_Z -1.0f
#define CIRCLE_RAD 2.3f

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

// Global spherical coord values
GLfloat azimuth = 45.0f;
GLfloat daz = 2.0f;
GLfloat elevation = 53.5f;
GLfloat del = 2.0f;
GLfloat radius = 1.732f;

/// Global state
GLfloat pyr_angle = 0.0;
GLdouble rpm = 10.0;
vec3 axis = {1.0f, 1.0f, 1.0f};
// Global spin variables - DO NOT MODIFY
GLfloat spin_theta = 0.0f;
GLboolean spin_flag = false;
GLfloat rev_theta = 0.0f;
GLboolean rev_flag = false;
GLdouble elTime = 0.0;
GLint mode = 0;

// Global screen dimensions
GLint ww = 640;
GLint hh = 480;
