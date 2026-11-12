// Model vertex arrays and buffer objects
enum VAO_IDs {Cube, Sphere, TexCube, Background, NumVAOs};
GLuint VAOs[NumVAOs];
GLuint ObjBuffers[NumVAOs][NumObjBuffers];
GLint numVertices[NumVAOs];
// Model files
const char * cubeFile = "../../common/models/unitcube.obj";
const char * sphereFile = "../../common/models/uv_sphere.obj";

void load_model(const char * filename, GLuint obj);
void build_texture_cube(GLuint obj);
void build_background(GLuint b_obj);

void build_geometry( )
{
    // Generate vertex arrays for objects
    glGenVertexArrays(NumVAOs, VAOs);

    // Load models
    load_model(cubeFile, Cube);
    load_model(sphereFile, Sphere);
    
    // Build texture cube
    build_texture_cube(TexCube);

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

void build_texture_cube(GLuint obj) {
    vector<vec4> vertices;
    vector<vec3> normals;
    vector<vec2> uvCoords;

    // Define 3D vertices for cube
    vertices = {
            vec4(0.5f,  -0.5f, 0.5f, 1.0f),    // front
            vec4(0.5f,  0.5f,  0.5f, 1.0f),
            vec4(-0.5f, 0.5f,  0.5f, 1.0f),
            vec4(-0.5f, 0.5f,  0.5f, 1.0f),
            vec4(-0.5f, -0.5f, 0.5f, 1.0f),
            vec4(0.5f,  -0.5f, 0.5f, 1.0f),
            vec4(-0.5f, -0.5f, -0.5f, 1.0f),   // back
            vec4(-0.5f, 0.5f,  -0.5f, 1.0f),
            vec4(0.5f,  0.5f,  -0.5f, 1.0f),
            vec4(0.5f,  0.5f,  -0.5f, 1.0f),
            vec4(0.5f,  -0.5f, -0.5f, 1.0f),
            vec4(-0.5f, -0.5f, -0.5f, 1.0f),
            vec4(0.5f, -0.5f, -0.5f, 1.0f),   // left
            vec4(0.5f, 0.5f, -0.5f, 1.0f),
            vec4(0.5f, 0.5f,  0.5f, 1.0f),
            vec4(0.5f, 0.5f,  0.5f, 1.0f),
            vec4(0.5f, -0.5f,  0.5f, 1.0f),
            vec4(0.5f, -0.5f, -0.5f, 1.0f),
            vec4(-0.5f,  -0.5f, -0.5f, 1.0f),   // right
            vec4(-0.5f,  -0.5f,  0.5f, 1.0f),
            vec4(-0.5f,  0.5f,  0.5f, 1.0f),
            vec4(-0.5f,  0.5f,  0.5f, 1.0f),
            vec4(-0.5f,  0.5f, -0.5f, 1.0f),
            vec4(-0.5f,  -0.5f, -0.5f, 1.0f),
            vec4(-0.5f, 0.5f,  -0.5f, 1.0f),   // top
            vec4(-0.5f, 0.5f,  0.5f, 1.0f),
            vec4(0.5f,  0.5f,  0.5f, 1.0f),
            vec4(0.5f,  0.5f,  0.5f, 1.0f),
            vec4(0.5f,  0.5f,  -0.5f, 1.0f),
            vec4(-0.5f, 0.5f,  -0.5f, 1.0f),
            vec4(-0.5f, -0.5f, -0.5f, 1.0f),   // bottom
            vec4(0.5f,  -0.5f, -0.5f, 1.0f),
            vec4(0.5f,  -0.5f, 0.5f, 1.0f),
            vec4(0.5f,  -0.5f, 0.5f, 1.0f),
            vec4(-0.5f, -0.5f, 0.5f, 1.0f),
            vec4(-0.5f, -0.5f, -0.5f, 1.0f)
    };

    normals = {
            vec3(0.0f,  0.0f, 1.0f),    // Front
            vec3(0.0f,  0.0f, 1.0f),
            vec3(0.0f,  0.0f, 1.0f),
            vec3(0.0f,  0.0f, 1.0f),
            vec3(0.0f,  0.0f, 1.0f),
            vec3(0.0f,  0.0f, 1.0f),
            vec3(1.0f,  0.0f, -1.0f),   // Back
            vec3(0.0f,  0.0f, -1.0f),
            vec3(0.0f,  0.0f, -1.0f),
            vec3(0.0f,  0.0f, -1.0f),
            vec3(0.0f,  0.0f, -1.0f),
            vec3(0.0f,  0.0f, -1.0f),
            vec3(1.0f,  0.0f, 0.0f),    // Left
            vec3(1.0f,  0.0f, 0.0f),
            vec3(1.0f,  0.0f, 0.0f),
            vec3(1.0f,  0.0f, 0.0f),
            vec3(1.0f,  0.0f, 0.0f),
            vec3(1.0f,  0.0f, 0.0f),
            vec3(-1.0f,  0.0f, 0.0f),    // Right
            vec3(-1.0f,  0.0f, 0.0f),
            vec3(-1.0f,  0.0f, 0.0f),
            vec3(-1.0f,  0.0f, 0.0f),
            vec3(-1.0f,  0.0f, 0.0f),
            vec3(-1.0f,  0.0f, 0.0f),
            vec3(0.0f,  1.0f, 0.0f),     // Top
            vec3(0.0f,  1.0f, 0.0f),
            vec3(0.0f,  1.0f, 0.0f),
            vec3(0.0f,  1.0f, 0.0f),
            vec3(0.0f,  1.0f, 0.0f),
            vec3(0.0f,  1.0f, 0.0f),
            vec3(0.0f,  -1.0f, 0.0f),     // Bottom
            vec3(0.0f,  -1.0f, 0.0f),
            vec3(0.0f,  -1.0f, 0.0f),
            vec3(0.0f,  -1.0f, 0.0f),
            vec3(0.0f,  -1.0f, 0.0f),
            vec3(0.0f,  -1.0f, 0.0f)
    };

    // TODO: Define texture coordinates for torso
    uvCoords = {
        // Front  (left panel)
        vec2(0.00f, 0.15f), vec2(0.00f, 0.85f), vec2(0.33f, 0.85f),
        vec2(0.33f, 0.85f), vec2(0.33f, 0.15f), vec2(0.00f, 0.15f),

        // Back   (middle panel)
        vec2(0.33f, 0.15f), vec2(0.33f, 0.85f), vec2(0.66f, 0.85f),
        vec2(0.66f, 0.85f), vec2(0.66f, 0.15f), vec2(0.33f, 0.15f),

        // Left   (right half of the right panel)
        vec2(0.83f, 0.15f), vec2(0.83f, 0.85f), vec2(1.00f, 0.85f),
        vec2(1.00f, 0.85f), vec2(1.00f, 0.15f), vec2(0.83f, 0.15f),

        // Right  (left half of the right panel)
        vec2(0.66f, 0.15f), vec2(0.66f, 0.85f), vec2(0.83f, 0.85f),
        vec2(0.83f, 0.85f), vec2(0.83f, 0.15f), vec2(0.66f, 0.15f),

        // Top    (any region is fine – using a bit of the front)
        vec2(0.00f, 0.15f), vec2(0.00f, 0.85f), vec2(0.33f, 0.85f),
        vec2(0.33f, 0.85f), vec2(0.33f, 0.15f), vec2(0.00f, 0.15f),

        // Bottom (any region is fine – using the black area)
        vec2(0.70f, 0.00f), vec2(0.70f, 0.10f), vec2(1.00f, 0.10f),
        vec2(1.00f, 0.10f), vec2(1.00f, 0.00f), vec2(0.70f, 0.00f)
    };

    // Set number of vertices
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

    vertices = {
            vec4(1.0f, 1.0f, 0.0f, 1.0f),
            vec4(-1.0f, 1.0f, 0.0f, 1.0f),
            vec4(-1.0f, -1.0f, 0.0f, 1.0f),
            vec4(-1.0f, -1.0f, 0.0f, 1.0f),
            vec4(1.0f, -1.0f, 0.0f, 1.0f),
            vec4(1.0f, 1.0f, 0.0f, 1.0f),
    };

    // TODO: Define texture coordinates for background
    uvCoords = {
        vec2(1.0f, 1.0f),
        vec2(0.0f, 1.0f),
        vec2(0.0f, 0.0f),
        vec2(0.0f, 0.0f),
        vec2(1.0f, 0.0f),
        vec2(1.0f, 1.0f)
    };

    numVertices[b_obj] = vertices.size();

    glBindVertexArray(VAOs[b_obj]);
    glGenBuffers(NumObjBuffers, ObjBuffers[b_obj]);
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[b_obj][PosBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*posCoords*numVertices[b_obj], vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[b_obj][TexBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*texCoords*numVertices[b_obj], uvCoords.data(), GL_STATIC_DRAW);
}
