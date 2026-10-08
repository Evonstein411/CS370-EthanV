// Model vertex arrays and buffer objects
enum VAO_IDs {Cube, Sphere, NumVAOs};
GLuint VAOs[NumVAOs];
GLuint ObjBuffers[NumVAOs][NumObjBuffers];
GLint numVertices[NumVAOs];

// Color buffers
enum Color_Buffer_IDs {CubeGradient, SphereYellow, NumColorBuffers};
GLuint ColorBuffers[NumColorBuffers];

// Model files
const char * cubeFile = "../../common/models/unitcube.obj";
const char * sphereFile = "../../common/models/sphere.obj";

void load_model(const char * filename, GLuint obj);
void build_solid_color_buffer(GLuint num_vertices, vec4 color, GLuint buffer);
void build_cube_gradient(GLuint num_vertices, GLuint buffer);

void build_geometry( )
{
    // Generate vertex arrays for objects
    glGenVertexArrays(NumVAOs, VAOs);

    // Load models
    load_model(cubeFile, Cube);
    load_model(sphereFile, Sphere);
    
        // Generate color buffers
    glGenBuffers(NumColorBuffers, ColorBuffers);

    // Define gradient colors for cube
    build_cube_gradient(numVertices[Cube], CubeGradient);

    // Define sphere vertex colors (yellow)
    build_solid_color_buffer(numVertices[Sphere], vec4(1.0f, 1.0f, 0.0f, 1.0f), SphereYellow);
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

void build_solid_color_buffer(GLuint num_vertices, vec4 color, GLuint buffer) {
    vector<vec4> obj_colors;
    for (int i = 0; i < num_vertices; i++) {
        obj_colors.push_back(color);
    }

    glBindBuffer(GL_ARRAY_BUFFER, ColorBuffers[buffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*colCoords*num_vertices, obj_colors.data(), GL_STATIC_DRAW);
}

void build_cube_gradient(GLuint num_vertices, GLuint buffer) {
    // Define cube vertex colors (gradient)
    vector<vec4> grad;
    for (int i = 0; i < num_vertices; i++) {
        if (i % 6 == 0) {
            grad.push_back(vec4(1.0f, 0.0f, 0.0f, 1.0f));
        } else if (i % 6 == 1) {
            grad.push_back(vec4(0.0f, 1.0f, 0.0f, 1.0f));
        } else if (i % 6 == 2) {
            grad.push_back(vec4(0.0f, 0.0f, 1.0f, 1.0f));
        } else if (i % 6 == 3) {
            grad.push_back(vec4(1.0f, 1.0f, 0.0f, 1.0f));
        } else if (i % 6 == 4) {
            grad.push_back(vec4(0.0f, 1.0f, 1.0f, 1.0f));
        } else if (i % 6 == 5) {
            grad.push_back(vec4(1.0f, 0.0f, 1.0f, 1.0f));
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, ColorBuffers[buffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*colCoords*num_vertices, grad.data(), GL_STATIC_DRAW);
}

