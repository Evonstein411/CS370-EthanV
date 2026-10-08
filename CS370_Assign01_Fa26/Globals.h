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

// Global object variables
GLfloat fan_angle = 0.0;
GLdouble rpm = 10.0;
GLint dir = 1;
GLdouble elTime = 0.0;
GLboolean animate = false;
GLfloat sun_angle = 40.0f;
GLfloat sun_deg_per_sec = 20.0f;
GLfloat sun_x = 0.0f;
GLfloat sun_y = 0.0f;
GLboolean isDay = true;
const int NUM_STARS = 50;
vec2  star_pos[NUM_STARS];
float star_size[NUM_STARS];


// Global screen dimensions
GLint ww = 1280;
GLint hh = 960;
