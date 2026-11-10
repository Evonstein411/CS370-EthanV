using namespace vmath;
using namespace std;

// Buffer enums
enum ObjBuffer_IDs {PosBuffer, NormBuffer, TexBuffer, TangBuffer, BiTangBuffer, NumObjBuffers};
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
enum CameraMode {ORTHO, PERSPECTIVE};
CameraMode camera_mode = ORTHO;

//Global camera state
vec3 eye = {3.0f, 3.0f, 3.0f};
vec3 center = {0.0f, 0.0f, 0.0f};
vec3 up = {0.0f, 1.0f, 0.0f};
GLfloat azimuth = 320.0f;
GLfloat elevation = 60.0f;
GLfloat radius = 17.5f;
GLfloat del = 2.0f;
GLfloat daz = 2.0f;
GLfloat dr = 0.1f;
GLfloat min_radius = 0.5f;

//Ortho camera
vec3 orthoEye = {3.0f, 3.0f, 3.0f};
vec3 orthoCenter = {0.0f, 0.0f, 0.0f};
GLfloat orthoAzimuth = 320.0f;
GLfloat orthoElevation = 60.0f;
GLfloat orthoRadius = 17.5f;

//Perspective (first person) camera
vec3 fpsEye = {0.0f, 2.0f, 10.0f};
GLfloat fpsAzimuth = 0.0f;
GLfloat fpsElevation = 0.0f;
GLfloat fpsStep = 0.25f;
GLfloat fpsTurn = 2.0f;

// Global object variables
GLfloat sphere_angle = 0.0;
GLdouble elTime = 0.0;
GLdouble rpm = 10.0;
vec3 axis = {0.0f, 1.0f, 0.0f};
GLboolean animate = true;
GLboolean dirty = false;

vec4 backgroundColor = {0.05f, 0.05f, 0.05f, 1.0f};

// Global screen dimensions
GLint ww = 1640;
GLint hh = 960;


