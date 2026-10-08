// Model vertex arrays and buffer objects
enum VAO_IDs {Sphere, Background, NumVAOs};
GLuint VAOs[NumVAOs];
GLuint ObjBuffers[NumVAOs][NumObjBuffers];
GLint numVertices[NumVAOs];
// Model files
const char * sphereFile = "../../common/models/uv_sphere.obj";

void load_model(const char * filename, GLuint obj);
void build_background(GLuint b_obj);

void build_geometry( )
{
    // Generate vertex arrays for objects
    glGenVertexArrays(NumVAOs, VAOs);

    // Load models
    load_model(sphereFile, Sphere);

    // Build background
    build_background(Background);
}

void load_model(const char * filename, GLuint obj) {
    vector<vec4> vertices;
    vector<vec2> uvCoords;
    vector<vec3> normals;

    // Load model and set number of vertices
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

void build_background(GLuint b_obj) {
    vector<vec4> vertices;
    vector<vec2> uvCoords;

    // Define background vertices
    vertices = {
            {1.0f, 1.0f, 0.0f, 1.0f},
            {-1.0f, 1.0f, 0.0f, 1.0f},
            {-1.0f, -1.0f, 0.0f, 1.0f},
            {-1.0f, -1.0f, 0.0f, 1.0f},
            {1.0f, -1.0f, 0.0f, 1.0f},
            {1.0f, 1.0f, 0.0f, 1.0f},
    };

    // TODO: Define texture coordinates
    uvCoords = {
            {2.0f, 2.0f},
            {-1.0f, 2.0f},
            {-1.0f, -1.0f},
            {-1.0f, -1.0f},
            {2.0f, -1.0f},
            {2.0f, 2.0f},
    };

    numVertices[b_obj] = vertices.size();

    glBindVertexArray(VAOs[b_obj]);
    glGenBuffers(NumObjBuffers, ObjBuffers[b_obj]);
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[b_obj][PosBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*posCoords*numVertices[b_obj], vertices.data(), GL_STATIC_DRAW);
    // TODO: Bind texture coordinate buffer and load data
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[b_obj][TexBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*texCoords*numVertices[b_obj], uvCoords.data(), GL_STATIC_DRAW);
}
