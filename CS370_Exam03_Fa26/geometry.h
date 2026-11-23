// Model vertex arrays and buffer objects
enum VAO_IDs {Cube, Cylinder, Cone, Octahedron, Circle, Carpet, Turkey, Tray, TableCube, Present, Background, NumVAOs};
GLuint VAOs[NumVAOs];
GLuint ObjBuffers[NumVAOs][NumObjBuffers];
GLint numVertices[NumVAOs];

// Model files
const char * cubeFile = "../../common/models/unitcube.obj";
const char * cylinderFile = "../../common/models/cylinder.obj";
const char * coneFile = "../../common/models/cone.obj";
const char * octFile = "../../common/models/octahedron.obj";
const char * circleFile = "../models/circle.obj";
const char * trayFile = "../models/tray.obj";
const char * turkeyFile = "../models/turkey.obj";
const char * presentFile = "../models/present.obj";

void build_carpet(GLuint c_obj);
void build_table(GLuint t_obj);
void build_background(GLuint b_obj);
void load_model(const char * filename, GLuint obj);

void build_geometry( )
{
    // Generate vertex arrays for objects
    glGenVertexArrays(NumVAOs, VAOs);

    // Load objects
    load_model(cylinderFile,Cylinder);
    load_model(coneFile,Cone);
    load_model(cubeFile,Cube);
    load_model(octFile, Octahedron);
    load_model(circleFile, Circle);
    load_model(trayFile, Tray);
    load_model(turkeyFile, Turkey);
    load_model(presentFile, Present);

    build_background(Background);
    build_table(TableCube);
    build_carpet(Carpet);
}

void build_carpet(GLuint c_obj) {
    // Carpet geometry
    vector<vec4> vertices;
    vector<vec2> uvCoords;
    vector<vec3> indices;

    vertices = {
            vec4(1.0f, 0.0f, 1.0f, 1.0f),
            vec4(1.0f, 0.0f, -1.0f, 1.0f),
            vec4(-1.0f, 0.0f, -1.0f, 1.0f),
            vec4(-1.0f, 0.0f, 1.0f, 1.0f),
    };

    /******************************************/
    /*       INSERT (b) CODE HERE             */
    /******************************************/
    // TODO: Modify carpet texture coordinates
    uvCoords = {
        vec2(1.0f, 1.0f),
        vec2(1.0f, -1.0f),
        vec2(-1.0f, -1.0f),
        vec2(-1.0f, 1.0f),
    };

    // Define face indices
    indices = {
            {0, 1, 2},
            {2, 3, 0},
    };
    int numFaces = indices.size();

    // Create object vertices and colors from faces
    vector<vec4> obj_vertices;
    vector<vec2> obj_uvs;
    for (int i = 0; i < numFaces; i++) {
        for (int j = 0; j < 3; j++) {
            obj_vertices.push_back(vertices[indices[i][j]]);
            obj_uvs.push_back(uvCoords[indices[i][j]]);
        }
    }

    // Set numVertices as total number of INDICES
    numVertices[c_obj] = 3*numFaces;

    // Generate object buffers for obj
    glGenBuffers(NumObjBuffers, ObjBuffers[c_obj]);

    // Bind and load object buffers for obj
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[c_obj][PosBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*posCoords*numVertices[c_obj], obj_vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[c_obj][TexBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*texCoords*numVertices[c_obj], obj_uvs.data(), GL_STATIC_DRAW);
}

void build_table(GLuint t_obj) {
    vector<vec4> vertices;
    vector<vec3> normals;
    vector<vec2> uvCoords;
    vector<ivec3> indices;

    // Define vertices - DO NOT MODIFY
    vertices = {
            vec4(-0.5f, 0.5f,  -0.5f, 1.0f),   // top
            vec4(-0.5f, 0.5f,  0.5f, 1.0f),
            vec4(0.5f,  0.5f,  0.5f, 1.0f),
            vec4(0.5f,  0.5f,  -0.5f, 1.0f),
            vec4(-0.5f,  -0.5f, 0.5f, 1.0f),   // front
            vec4(0.5f,  -0.5f,  0.5f, 1.0f),
            vec4(0.5f, 0.5f,  0.5f, 1.0f),
            vec4(-0.5f, 0.5f, 0.5f, 1.0f),
            vec4(-0.5f, -0.5f, -0.5f, 1.0f),   // back
            vec4(-0.5f, 0.5f,  -0.5f, 1.0f),
            vec4(0.5f,  0.5f,  -0.5f, 1.0f),
            vec4(0.5f,  -0.5f, -0.5f, 1.0f),
            vec4(0.5f, -0.5f, -0.5f, 1.0f),    // left
            vec4(0.5f, 0.5f, -0.5f, 1.0f),
            vec4(0.5f, 0.5f,  0.5f, 1.0f),
            vec4(0.5f, -0.5f,  0.5f, 1.0f),
            vec4(-0.5f,  -0.5f, -0.5f, 1.0f),  // right
            vec4(-0.5f,  -0.5f,  0.5f, 1.0f),
            vec4(-0.5f,  0.5f,  0.5f, 1.0f),
            vec4(-0.5f,  0.5f, -0.5f, 1.0f),
            vec4(-0.5f, -0.5f, -0.5f, 1.0f),   // bottom
            vec4(0.5f,  -0.5f, -0.5f, 1.0f),
            vec4(0.5f,  -0.5f, 0.5f, 1.0f),
            vec4(-0.5f, -0.5f, 0.5f, 1.0f),
    };

    uvCoords = {
        /******************************************/
        /*       INSERT (j) CODE HERE             */
        /******************************************/
        // TODO: Modify top texture coordinates
        vec2(-0.5f, -0.5f),    // top
        vec2(-0.5f, 0.5f),
        vec2(0.5f, 0.5f),
        vec2(0.5f, 0.5f),

        // TODO: Modify front texture coordinates
        vec2(-0.5f, -0.5f),   // front
        vec2(0.5f, -0.5f),
        vec2(0.5f, 0.5f),
        vec2(-0.5f, 0.5f),

        /******************************************/
        /*       INSERT (s) CODE HERE (BONUS)     */
        /******************************************/
        // TODO: Modify remaining texture coordinates
        vec2(0.0f, 0.0f),    // back
        vec2(0.0f, 0.0f),
        vec2(0.0f, 0.0f),
        vec2(0.0f, 0.0f),
        vec2(0.0f, 0.0f),    // left
        vec2(0.0f, 0.0f),
        vec2(0.0f, 0.0f),
        vec2(0.0f, 0.0f),
        vec2(0.0f, 0.0f),    // right
        vec2(0.0f, 0.0f),
        vec2(0.0f, 0.0f),
        vec2(0.0f, 0.0f),
        vec2(0.0f, 0.0f),    // bottom
        vec2(0.0f, 0.0f),
        vec2(0.0f, 0.0f),
        vec2(0.0f, 0.0f),
    };

    // Define face indices (ensure proper orientation)
    indices = {
            {0, 1, 2},     // Top
            {2, 3, 0},
            {4, 5, 6},     // Front
            {6, 7, 4},
            {8, 9, 10},    // Back
            {10, 11, 8},
            {12, 13, 14},  // Left
            {14, 15, 12},
            {16, 17, 18},  // Right
            {18, 19, 16},
            {20, 21, 22},  // Bottom
            {22, 23, 20},
    };
    int numFaces = indices.size();

    // Create object vertices and colors from faces
    vector<vec4> obj_vertices;
    vector<vec3> obj_normals;
    vector<vec2> obj_uvs;
    for (int i = 0; i < numFaces; i++) {
        for (int j = 0; j < 3; j++) {
            obj_vertices.push_back(vertices[indices[i][j]]);
            obj_normals.push_back(vec3(0.0f, 1.0f, 0.0f));
            obj_uvs.push_back(uvCoords[indices[i][j]]);
        }
    }

    // Set numVertices as total number of INDICES
    numVertices[t_obj] = 3*numFaces;

    // Generate object buffers for obj
    glGenBuffers(NumObjBuffers, ObjBuffers[t_obj]);

    // Bind and load object buffers for obj
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[t_obj][PosBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*posCoords*numVertices[t_obj], obj_vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[t_obj][NormBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*normCoords*numVertices[t_obj], obj_normals.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[t_obj][TexBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*texCoords*numVertices[t_obj], obj_uvs.data(), GL_STATIC_DRAW);
}

void build_background(GLuint b_obj) {
    // Background geometry
    vector<vec4> vertices;
    vector<vec2> uvCoords;
    vector<vec3> indices;

    vertices = {
            vec4(1.0f, 1.0f, 0.0f, 1.0f),
            vec4(-1.0f, 1.0f, 0.0f, 1.0f),
            vec4(-1.0f, -1.0f, 0.0f, 1.0f),
            vec4(1.0f, -1.0f, 0.0f, 1.0f),
    };

    /******************************************/
    /*       INSERT (q) CODE HERE             */
    /******************************************/
    // TODO: Modify background texture coordinates
    uvCoords = {
        vec2(0.0f, 0.0f),
        vec2(0.0f, 0.0f),
        vec2(0.0f, 0.0f),
        vec2(0.0f, 0.0f),
    };

    // Define face indices
    indices = {
            {0, 1, 2},
            {2, 3, 0},
    };
    int numFaces = indices.size();

    // Create object vertices and texture coords from faces
    vector<vec4> obj_vertices;
    vector<vec2> obj_uvs;
    for (int i = 0; i < numFaces; i++) {
        for (int j = 0; j < 3; j++) {
            obj_vertices.push_back(vertices[indices[i][j]]);
            obj_uvs.push_back(uvCoords[indices[i][j]]);
        }
    }

    // Set numVertices as total number of INDICES
    numVertices[b_obj] = 3*numFaces;

    // Generate object buffers for obj
    glGenBuffers(NumObjBuffers, ObjBuffers[b_obj]);

    // Bind and load object buffers for obj
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[b_obj][PosBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*posCoords*numVertices[b_obj], obj_vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[b_obj][TexBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*texCoords*numVertices[b_obj], obj_uvs.data(), GL_STATIC_DRAW);
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
