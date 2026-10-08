// Model vertex arrays and buffer objects
enum VAO_IDs {Cube, Sphere, Cylinder, Torus, NumVAOs};
GLuint VAOs[NumVAOs];
GLuint ObjBuffers[NumVAOs][NumObjBuffers];
GLint numVertices[NumVAOs];

// Color buffers
enum Color_Buffer_IDs {Red, Yellow, Blue, Green, NumColorBuffers};
GLuint ColorBuffers[NumColorBuffers];

// Model files
const char * cubeFile = "../../common/models/unitcube.obj";
const char * sphereFile = "../../common/models/sphere.obj";
const char * cylinderFile = "../../common/models/cylinder.obj";
const char * torusFile = "../../common/models/torus.obj";



void load_model(const char * filename, GLuint obj);
void build_solid_color_buffer(GLuint num_vertices, vec4 color, GLuint buffer);
void build_cube_gradient(GLuint num_vertices, GLuint buffer);

void build_geometry( )
{
    // Generate vertex arrays for objects
    glGenVertexArrays(NumVAOs, VAOs);

    // TODO: Load models
    load_model(cubeFile, Cube);
    load_model(sphereFile, Sphere);
    load_model(cylinderFile, Cylinder);
    load_model(torusFile, Torus);
    
    // Generate color buffers
    glGenBuffers(NumColorBuffers, ColorBuffers);

    // TODO: Define cube vertex colors
    build_solid_color_buffer(numVertices[Cube], vec4(1.0f, 1.0f, 0.0f, 1.0f), Yellow);

    // TODO: Define sphere vertex colors
    build_solid_color_buffer(numVertices[Sphere], vec4(1.0f, 0.0f, 0.0f, 1.0f), Red);

    build_solid_color_buffer(numVertices[Cylinder], vec4(0.0f, 0.0f, 1.0f, 1.0f), Blue);

    build_solid_color_buffer(numVertices[Torus], vec4(0.0f, 1.0f, 0.0f, 1.0f), Green);

}

void load_model(const char * filename, GLuint obj) {
    vector<vec4> vertices;
    vector<vec2> uvCoords;
    vector<vec3> normals;

    // TODO: Load model and set number of vertices
    loadOBJ(filename, vertices, uvCoords, normals);
    numVertices[obj] = vertices.size();

    // Create and load object buffers
    glGenBuffers(NumObjBuffers, ObjBuffers[obj]);
    glBindVertexArray(VAOs[obj]);
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][PosBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*posCoords*numVertices[obj], vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][NormBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*normCoords*numVertices[obj], normals.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][TexBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*texCoords*numVertices[obj], uvCoords.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void build_solid_color_buffer(GLuint num_vertices, vec4 color, GLuint buffer) {
    vector<vec4> obj_colors;
    for (int i = 0; i < num_vertices; i++) {
        obj_colors.push_back(color);
    }

    glBindBuffer(GL_ARRAY_BUFFER, ColorBuffers[buffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*colCoords*num_vertices, obj_colors.data(), GL_STATIC_DRAW);
}
