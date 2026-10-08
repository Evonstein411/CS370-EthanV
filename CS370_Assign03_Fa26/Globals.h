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
GLfloat elevation = 53.5f;
GLfloat del = 2.0f;
GLfloat radius = 2.0f;

// Global object variables
vec3 cube_pos = { -6.0f, 0.5f, 0.0f };
GLfloat cube_angle = 0.0;
GLdouble rpm = 10.0;
vec3 axis = {0.0f, 1.0f, 0.0f};
GLfloat cube_dir = 1.0f;
GLfloat cube_slps = 3.0f;
GLboolean cube_slide = true;
vec3 sphere_pos = { 0.0f, 1.0f, -0.5f };
GLfloat sphere_dir = 1.0f;
GLfloat sphere_bps = 3.0f;
GLboolean sphere_bounce = true;
vec3 pyr_pos = { 6.0f, 1.0f, -0.5f };
GLfloat pyr_ang = 0.0f;
GLfloat pyr_dps = 360.0f;
GLboolean pyr_spin = true;
GLdouble elTime = 0.0;

// Global screen dimensions
GLint ww = 640;
GLint hh = 480;
