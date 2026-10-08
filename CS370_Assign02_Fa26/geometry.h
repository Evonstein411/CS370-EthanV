// Model vertex arrays and buffer objects
enum VAO_IDs {Cube, Skybox, Cylinder, Cone, Torus, Axes, NumVAOs};
GLuint VAOs[NumVAOs];
GLuint ObjBuffers[NumVAOs][NumObjBuffers];
GLint numVertices[NumVAOs];

// Color buffers
enum Color_Buffer_IDs {BodyColor, DirtColor, GrassColor, SkyColor, RailColor, EngColor, TiesColor, SpokeColor, TireColor, BottomColor, TopColor,
    MiddleColor, AxesColor, StackColor, FunnelColor, WoodColor, LeafColor, NumColorBuffers};
GLuint ColorBuffers[NumColorBuffers];

// Model files
const char * cubeFile = "../../common/models/unitcube.obj";
const char * cylinderFile = "../../common/models/cylinder.obj";
const char * coneFile = "../../common/models/cone.obj";
const char * torusFile = "../../common/models/torus.obj";
const char * skyboxFile = "../../common/models/inverse_unitcube.obj";

// Axis length
GLfloat axis_length = 3.0f;

void load_model(const char * filename, GLuint obj);
void build_solid_color_buffer(GLuint num_vertices, vec4 color, GLuint buffer);
void build_axes(GLuint obj);

void build_geometry( )
{
    // Generate vertex arrays for objects
    glGenVertexArrays(NumVAOs, VAOs);

    // TODO: Load models
    load_model(cubeFile, Cube);
    load_model(cylinderFile, Cylinder);
    load_model(coneFile, Cone);
    load_model(torusFile, Torus);
    load_model(skyboxFile, Skybox);

    
    // Generate color buffers
    glGenBuffers(NumColorBuffers, ColorBuffers);

    // TODO: Create color buffers
    // Create body color (cube)
    build_solid_color_buffer(numVertices[Cylinder], ties_color, WoodColor);
    build_solid_color_buffer(numVertices[Cone], middle_color, LeafColor);


    build_solid_color_buffer(numVertices[Cube], body_color, BodyColor);
    build_solid_color_buffer(numVertices[Cube], eng_color, EngColor);

    build_solid_color_buffer(numVertices[Cube], rail_color, RailColor);
    build_solid_color_buffer(numVertices[Cube], ties_color, TiesColor);

    build_solid_color_buffer(numVertices[Cube], bottom_color, BottomColor);
    build_solid_color_buffer(numVertices[Cube], middle_color, MiddleColor);
    build_solid_color_buffer(numVertices[Cube], top_color, TopColor);

    build_solid_color_buffer(numVertices[Cube], dirt_color, DirtColor);
    build_solid_color_buffer(numVertices[Cube], grass_color, GrassColor);
    build_solid_color_buffer(numVertices[Skybox], sky_color, SkyColor);
    build_solid_color_buffer(numVertices[Cube], sky_color, SkyColor);

    build_solid_color_buffer(numVertices[Cone], stack_color, FunnelColor);
    build_solid_color_buffer(numVertices[Cylinder], stack_color, StackColor);

    build_solid_color_buffer(numVertices[Cube], wheel_color, SpokeColor);
    build_solid_color_buffer(numVertices[Torus], wheel_color, TireColor);





    // Build axes
    build_axes(Axes);
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

void build_axes(GLuint obj) {
    vector<vec4> vertices;
    vector<ivec3> indices;
    vector<vec4> colors;

    // Bind target vertex array object
    glBindVertexArray(VAOs[Axes]);

    // Define vertices for axes
    vertices = {
            {0.0, 0.0f, 0.0f, 1.0f},
            {axis_length, 0.0f, 0.0f, 1.0f},  // x-axis
            {0.0f, 0.0f, 0.0f, 1.0f},
            {0.0f, axis_length, 0.0f, 1.0f}, // y-axis
            {0.0f, 0.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, axis_length, 1.0f}, // z-axis
    };

    // Define axis colors (red - x, green - y, blue - z)
    colors.push_back(vec4(1.0f, 0.0f, 0.0f, 1.0f));
    colors.push_back(vec4(1.0f, 0.0f, 0.0f, 1.0f));
    colors.push_back(vec4(0.0f, 1.0f, 0.0f, 1.0f));
    colors.push_back(vec4(0.0f, 1.0f, 0.0f, 1.0f));
    colors.push_back(vec4(0.0f, 0.0f, 1.0f, 1.0f));
    colors.push_back(vec4(0.0f, 0.0f, 1.0f, 1.0f));

    // Set numVertices
    numVertices[obj] = 6;

    // Generate object buffer for table
    glGenBuffers(NumObjBuffers, ObjBuffers[obj]);

    // Bind axes positions
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][PosBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*posCoords*numVertices[obj], vertices.data(), GL_STATIC_DRAW);

    // Bind axes colors
    glBindBuffer(GL_ARRAY_BUFFER, ColorBuffers[AxesColor]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*colCoords*numVertices[obj], colors.data(), GL_STATIC_DRAW);
}
