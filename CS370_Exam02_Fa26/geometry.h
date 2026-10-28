// Model vertex arrays and buffer objects
enum VAO_IDs {Cube, Plate, Donut, Teapot, Mug, Coffee, Axes, NumVAOs};
GLuint VAOs[NumVAOs];
GLuint ObjBuffers[NumVAOs][NumObjBuffers];
GLint numVertices[NumVAOs];

// Color buffers
enum Color_Buffer_IDs {AxesColor, NumColorBuffers};
GLuint ColorBuffers[NumColorBuffers];

// Model files
const char * cubeFile = "../../common/models/unitcube.obj";
const char * donutFile = "../models/donut.obj";
const char * teapotFile = "../models/teapot.obj";
const char * mugFile = "../models/mug.obj";
const char * cylinderFile = "../../common/models/cylinder.obj";

void build_axes(GLuint a_obj, GLuint a_buff);
void load_model(const char * filename, GLuint obj);

void build_geometry( )
{
    // Generate vertex arrays for objects
    glGenVertexArrays(NumVAOs, VAOs);
    // Generate color buffers
    glGenBuffers(NumColorBuffers, ColorBuffers);

    // Load objects
    load_model(cubeFile,Cube);
    load_model(cylinderFile,Plate);
    load_model(donutFile,Donut);
    load_model(teapotFile,Teapot);
    load_model(mugFile,Mug);
    load_model(cylinderFile,Coffee);

    build_axes(Axes, AxesColor);
}

void build_axes(GLuint a_obj, GLuint a_buff) {
    vector<vec4> vertices;
    vector<vec4> colors;

    // Bind target vertex array object
    glBindVertexArray(VAOs[a_obj]);

    // Define vertices for axes
    vertices = {
            {0.0, 0.0f, 0.0f, 1.0f},
            {AXIS_LEN, 0.0f, 0.0f, 1.0f},  // x-axis
            {0.0f, 0.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, AXIS_LEN, 1.0f}, // z-axis
    };

    // Define axis colors (red - x, green - y, blue - z)
    colors.push_back(vec4(1.0f, 0.0f, 0.0f, 1.0f));
    colors.push_back(vec4(1.0f, 0.0f, 0.0f, 1.0f));
    colors.push_back(vec4(0.0f, 0.0f, 1.0f, 1.0f));
    colors.push_back(vec4(0.0f, 0.0f, 1.0f, 1.0f));

    // Set numVertices
    numVertices[a_obj] = vertices.size();

    // Generate object buffer for table
    glGenBuffers(NumObjBuffers, ObjBuffers[a_obj]);

    // Bind axes positions
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[a_obj][PosBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*posCoords*numVertices[a_obj], vertices.data(), GL_STATIC_DRAW);

    // Bind axes colors
    glBindBuffer(GL_ARRAY_BUFFER, ColorBuffers[a_buff]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*colCoords*numVertices[a_obj], colors.data(), GL_STATIC_DRAW);
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
