// Model vertex arrays and buffer objects
enum VAO_IDs {Square, NumVAOs};
GLuint VAOs[NumVAOs];
GLuint ObjBuffers[NumVAOs][NumObjBuffers];
GLint numVertices[NumVAOs];

void build_square(GLuint obj);

void build_geometry( )
{
    // TODO: Generate vertex arrays for objects
    glGenVertexArrays(NumVAOs, VAOs);
    
    // TODO: Build square manually
    build_square(Square);
}

void build_square(GLuint obj) {
    vector<vec2> vertices;

    // TODO: Bind vertex array for obj
    glBindVertexArray(VAOs[obj]);

    // TODO: Define vertices (ensure proper orientation)
    vertices = {
            {-0.5f,  -0.5f},
            {0.5f,   0.5f},
            {-0.5f,  0.5f},
            {-0.50f, -0.5f},
            {0.5f,   -0.5f},
            {0.5f,   0.5f},
    };

    // TODO: Store number of vertices
    numVertices[obj] = vertices.size();

    // TODO: Generate object buffers for obj
    glGenBuffers(NumObjBuffers, ObjBuffers[obj]);

    // TODO: Bind and load position object buffer for obj
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][PosBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*posCoords*numVertices[obj], vertices.data(), GL_STATIC_DRAW);
}
