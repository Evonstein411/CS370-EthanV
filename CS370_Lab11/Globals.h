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
vec3 eye = {0.0f, 0.0f, 4.0f};
vec3 center = {0.0f, 0.0f, 0.0f};
vec3 up = {0.0f, 1.0f, 0.0f};

// Global object variables
vec3 sphere_pos = {0.0f,0.0f,-0.2f};
vec3 torus_pos = {0.0f,-2.8f,-1.5f};
GLfloat torus_theta = 0.0f;
GLboolean bounce_sphere = true;
GLboolean roll_torus = true;
GLfloat SPHERE_STEP = 6.0f;
GLfloat SPHERE_MIN = -0.2f;
GLfloat SPHERE_MAX = 2.0f;
GLint sphere_dir = 1;
GLfloat TORUS_STEP = 90.0f;
GLdouble elTime = 0.0;

// Global screen dimensions
GLint ww = 640;
GLint hh = 480;
