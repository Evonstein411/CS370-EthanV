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
mat4 projMatrix;
mat4 cameraMatrix;
mat4 modelMatrix;

// Global screen dimensions
GLint ww = 640;
GLint hh = 480;
