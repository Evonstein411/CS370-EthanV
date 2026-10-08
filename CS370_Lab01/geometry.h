// Model vertex arrays and buffer objects
enum VAO_IDs {Square, Triangle, Circle, NumVAOs};
GLuint VAOs[NumVAOs];
GLuint ObjBuffers[NumVAOs][NumObjBuffers];
GLint numVertices[NumVAOs];

void build_square(GLuint obj);
void build_triangle(GLuint obj);
void build_circle(GLuint obj);

void build_geometry( )
{
    // TODO: Generate vertex arrays for objects
   glGenVertexArrays(NumVAOs, VAOs);
    
    // TODO: Build square manually
    build_square(Square);
    build_triangle(Triangle);
    build_circle(Circle);

    
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

void build_triangle(GLuint obj) {
    vector<vec2> vertices;
    glBindVertexArray(VAOs[obj]);

    vertices = {
     {-0.25, -0.25},
     {0.25,  -0.25},
     {0.25, 0.25},
    };

    numVertices[obj] = vertices.size();
    glGenBuffers(NumObjBuffers, ObjBuffers[obj]);

    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][PosBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*posCoords*numVertices[obj], vertices.data(), GL_STATIC_DRAW);
}

void build_circle(GLuint obj) {
    vector<vec2> vertices;
    glBindVertexArray(VAOs[obj]);
    int numSegments = 40;
    float radius = 0.25f;
    vec2 center = vec2(0.0f, 0.0f);

    for (int i = 0; i < numSegments; i++) {
        float angle0 = 2.0f * M_PI * i / numSegments;
        float angle1 = 2.0f * M_PI * (i + 1) / numSegments;

        vec2 p0 = vec2(radius * cos(angle0), radius * sin(angle0));
        vec2 p1 = vec2(radius * cos(angle1), radius * sin(angle1));

        vertices.push_back(center);
        vertices.push_back(p0);
        vertices.push_back(p1);
    }

    numVertices[obj] = vertices.size();
    glGenBuffers(NumObjBuffers, ObjBuffers[obj]);

    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][PosBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*posCoords*numVertices[obj], vertices.data(), GL_STATIC_DRAW);
}