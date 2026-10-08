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
GLfloat hex_angle = 0.0;
GLfloat hex_x = 0.0;
GLfloat hex_y = 0.0;
GLfloat delta = 0.1;
GLdouble rpm = 10.0;
GLint dir = 1;
GLdouble elTime = 0.0;
GLboolean animate = true;

// Global screen dimensions
GLint ww = 640;
GLint hh = 480;
