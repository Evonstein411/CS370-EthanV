// Model vertex arrays and buffer objects
enum VAO_IDs {Hexagon, NumVAOs};
GLuint VAOs[NumVAOs];
GLuint ObjBuffers[NumVAOs][NumObjBuffers];
GLint numVertices[NumVAOs];

// Color buffers
enum Color_Buffer_IDs {HexRed, HexGreen, HexBlue, HexYellow, HexPurple, NumColorBuffers};
GLuint ColorBuffers[NumColorBuffers];

void build_hexagon(GLuint obj);
void build_solid_color_buffer(GLuint num_vertices, vec4 color, GLuint buffer);

void build_geometry( )
{
    // Generate vertex arrays for objects
    glGenVertexArrays(NumVAOs, VAOs);
    
    // Build hexagon manually
    build_hexagon(Hexagon);
    
    // Generate color buffers
    glGenBuffers(NumColorBuffers, ColorBuffers);
    
    // Build hexagon color buffer (red)
    build_solid_color_buffer(numVertices[Hexagon], vec4(1.0f, 0.0f, 0.0f, 1.0f), HexRed);
    
    // TODO: Build additional color buffers
    build_solid_color_buffer(numVertices[Hexagon], vec4(0.0f, 1.0f, 0.0f, 1.0f), HexGreen);
    build_solid_color_buffer(numVertices[Hexagon], vec4(0.0f, 0.0f, 1.0f, 1.0f), HexBlue);
    build_solid_color_buffer(numVertices[Hexagon], vec4(1.0f, 1.0f, 0.0f, 1.0f), HexYellow);
    build_solid_color_buffer(numVertices[Hexagon], vec4(1.0f, 0.0f, 1.0f, 1.0f), HexPurple);



}

void build_hexagon(GLuint obj) {
    vector<vec2> vertices;
    vector<ivec3> indices;
    vector<vec4> red;

    // Bind vertex array for obj
    glBindVertexArray(VAOs[obj]);

    // Define vertices (no particular orientation)
    vertices = {
            //{0.0f, 0.0f},  // non-convex but DOES work
            {1.0f, 0.0f},
            {0.5f, 0.866f},
            {-0.5f, 0.866f},
            {-1.0f, 0.0f},
            {-0.5f, -0.866f},
            {0.5f, -0.866f},
            //{0.0f, 0.0f}   // non-convex but DOESN'T work
    };

    // Define face indices (ensure proper orientation)
    indices = {
            {0, 1, 2},
            {2, 3, 4},
            {4, 5, 0},
            {0, 2, 4}
    };
    int numFaces = indices.size();

    // Create object vertices and colors from faces
    vector<vec4> obj_vertices;
    for (int i = 0; i < numFaces; i++) {
        for (int j = 0; j < 3; j++) {
        	obj_vertices.push_back(vec4(vertices[indices[i][j]][0],
        	vertices[indices[i][j]][1], 
        	0.0f, 
        	1.0f));
        }
    }

    // Set numVertices as total number of INDICES
    numVertices[obj] = 3*numFaces;

    // Generate object buffers for obj
    glGenBuffers(NumObjBuffers, ObjBuffers[obj]);

    // Bind and load position object buffer for obj
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][PosBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*posCoords*numVertices[obj], obj_vertices.data(), GL_STATIC_DRAW);
}

void build_solid_color_buffer(GLuint num_vertices, vec4 color, GLuint buffer) {
    // Create object colors
    vector<vec4> obj_colors;
    for (int i = 0; i < num_vertices; i++) {
        obj_colors.push_back(color);
    }

    // Bind and load color buffers
    glBindBuffer(GL_ARRAY_BUFFER, ColorBuffers[buffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*colCoords*num_vertices, obj_colors.data(), GL_STATIC_DRAW);
}
