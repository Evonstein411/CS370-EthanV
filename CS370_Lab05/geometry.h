// Model vertex arrays and buffer objects
enum VAO_IDs {Cube, NumVAOs};
GLuint VAOs[NumVAOs];
GLuint ObjBuffers[NumVAOs][NumObjBuffers];
GLint numVertices[NumVAOs];

// Color buffers
enum Color_Buffer_IDs {CubeGradient, NumColorBuffers};
GLuint ColorBuffers[NumColorBuffers];

void build_cube(GLuint obj);
void build_gradient_color_buffer(vector<ivec3> indices, vector<vec4> colors, GLuint c_buff);

void build_geometry( )
{
    // Generate vertex arrays for objects
    glGenVertexArrays(NumVAOs, VAOs);
    // Generate color buffers
    glGenBuffers(NumColorBuffers, ColorBuffers);

	// Build cube manually
    build_cube(Cube);
}

void build_cube(GLuint obj) {
    vector<vec4> vertices;
    vector<ivec3> indices;

    // Bind vertex array for obj
    glBindVertexArray(VAOs[obj]);

    // Define 3D (homogeneous) vertices for cube
    vertices = {
            {-0.5f, -0.5f, -0.5f, 1.0f},
            {0.5f, -0.5f, -0.5f, 1.0f},
            {0.5f, -0.5f, 0.5f, 1.0f},
            {-0.5f, -0.5f, 0.5f, 1.0f},
            {-0.5f, 0.5f, -0.5f, 1.0f},
            {0.5f, 0.5f, -0.5f, 1.0f},
            {0.5f, 0.5f, 0.5f, 1.0f},
            {-0.5f, 0.5f, 0.5f, 1.0f}
    };

    // TODO: Define face indices (ensure proper orientation)
    indices = {
        {4, 7, 6}, //top
        {6, 5, 4},
        {0, 1, 2}, //bottom
        {2, 3, 0},
        {0, 3, 7}, //left
        {7, 4, 0},
        {1, 5, 6}, //right
        {6, 2, 1},
        {2, 6, 7}, //front
        {7, 3, 2},
        {0, 4, 5}, //back
        {5, 1, 0},
    };

        int numFaces = indices.size();
    // Set numVertices as total number of INDICES
    numVertices[obj] = 3*numFaces;

	// Create gradient colors
    vector<vec4> colors;
    // Define vertex colors
    colors = {
            {0.0f, 0.0f, 0.0f, 1.0f},
            {1.0f, 0.0f, 0.0f, 1.0f},
            {1.0f, 0.0f, 1.0f, 1.0f},
            {0.0f, 0.0f, 1.0f, 1.0f},
            {0.0f, 1.0f, 0.0f, 1.0f},
            {1.0f, 1.0f, 0.0f, 1.0f},
            {1.0f, 1.0f, 1.0f, 1.0f},
            {0.0f, 1.0f, 1.0f, 1.0f}
    };
    // Build gradient color buffer
    build_gradient_color_buffer(indices, colors, CubeGradient);

    // Create object vertices and colors from faces
    vector<vec4> obj_vertices;
    for (int i = 0; i < numFaces; i++) {
        for (int j = 0; j < 3; j++) {
            obj_vertices.push_back(vertices[indices[i][j]]);
        }
    }

    // Generate object buffers for obj
    glGenBuffers(NumObjBuffers, ObjBuffers[obj]);

    // Bind and load position object buffer for obj
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][PosBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*posCoords*numVertices[obj], obj_vertices.data(), GL_STATIC_DRAW);
}

void build_gradient_color_buffer(vector<ivec3> indices, vector<vec4> colors, GLuint c_buff) {
    int num_faces = indices.size();
    // Create object colors
    vector<vec4> obj_colors;
    for (int i = 0; i < num_faces; i++) {
        for (int j = 0; j < 3; j++) {
            obj_colors.push_back(colors[indices[i][j]]);
        }
    }

    // Bind and load color buffers
    glBindBuffer(GL_ARRAY_BUFFER, ColorBuffers[c_buff]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*colCoords*3*num_faces, obj_colors.data(), GL_STATIC_DRAW);
}
