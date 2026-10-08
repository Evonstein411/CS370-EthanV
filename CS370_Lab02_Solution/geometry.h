// Model vertex arrays and buffer objects
enum VAO_IDs {Hexagon, NumVAOs};
GLuint VAOs[NumVAOs];
GLuint ObjBuffers[NumVAOs][NumObjBuffers];
GLint numVertices[NumVAOs];

// Color buffers
enum Color_Buffer_IDs {HexGradient, NumColorBuffers};
GLuint ColorBuffers[NumColorBuffers];

void build_hexagon(GLuint obj, GLuint h_buff);
void build_gradient_color_buffer(vector<ivec3> indices, vector<vec4> colors, GLuint c_buff);

void build_geometry( )
{
    // Generate vertex arrays for objects
    glGenVertexArrays(NumVAOs, VAOs);
    
    // Generate color buffers
    glGenBuffers(NumColorBuffers, ColorBuffers);

    // TODO: Build hexagon manually
    build_hexagon(Hexagon, HexGradient);
}

void build_hexagon(GLuint obj, GLuint h_buff) {
    vector<vec2> vertices;
    vector<ivec3> indices;

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

    // TODO: Define face indices (ensure proper orientation)
    indices = {
            {0, 1, 2},
            {2, 3, 4},
            {4, 5, 0},
            {0, 2, 4},
    };
    int numFaces = indices.size();
    // TODO: Set numVertices as total number of INDICES (3*number of faces)
    numVertices[obj] = 3*numFaces;

	// Create gradient colors
    vector<vec4> colors;
    // TODO: Define colors per vertex
    colors = {
            {0.0f, 0.0f, 1.0f, 1.0f},
            {1.0f, 0.0f, 0.0f, 1.0f},
            {1.0f, 0.0f, 0.0f, 1.0f},
            {0.0f, 1.0f, 0.0f, 1.0f},
            {0.0f, 1.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f, 1.0f}
    };
    // TODO: Build gradient color buffer
    build_gradient_color_buffer(indices, colors, h_buff);

    // Create object vertices from faces
    vector<vec4> obj_vertices;
    for (int i = 0; i < numFaces; i++) {
        for (int j = 0; j < 3; j++) {
            obj_vertices.push_back(vec4(vertices[indices[i][j]][0],
        	vertices[indices[i][j]][1], 
        	0.0f, 
        	1.0f));
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

    // TODO: Bind and load color buffers
    glBindBuffer(GL_ARRAY_BUFFER, ColorBuffers[c_buff]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*colCoords*3*num_faces, obj_colors.data(), GL_STATIC_DRAW);
}
