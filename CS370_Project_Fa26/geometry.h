// Model vertex arrays and buffer objects
enum VAO_IDs {Cube, HUDQuad, HUDTextQuad, NumVAOs};
GLuint VAOs[NumVAOs];
GLuint ObjBuffers[NumVAOs][NumObjBuffers];
GLint numVertices[NumVAOs];

// Color buffers
enum Color_Buffer_IDs {CubeRed, HUDGray, HUDHighlighted, HUDTextField, NumColorBuffers};
GLuint ColorBuffers[NumColorBuffers];

// Model files
const char * cubeFile = "../../common/models/unitcube.obj";

void load_model(const char * filename, GLuint obj);
void build_solid_color_buffer(GLuint num_vertices, vec4 color, GLuint buffer);
void build_hud_quad(GLuint obj);


void build_geometry( )
{
    // Generate vertex arrays for objects
    glGenVertexArrays(NumVAOs, VAOs);

    //build objects
    build_hud_quad(HUDQuad);
    build_hud_quad(HUDTextQuad);

    // Load models
    load_model(cubeFile, Cube);
    
    // Generate color buffers
    glGenBuffers(NumColorBuffers, ColorBuffers);

    // Build color buffers
    build_solid_color_buffer(numVertices[Cube], vec4(1.0f, 0.0f, 0.0f, 1.0f), CubeRed);
    build_solid_color_buffer(numVertices[HUDQuad], vec4(0.4f, 0.4f, 0.4f, 0.6f), HUDGray);
    build_solid_color_buffer(numVertices[HUDQuad], vec4(0.8f, 0.8f, 0.8f, 0.6f), HUDHighlighted);
    build_solid_color_buffer(numVertices[HUDQuad], vec4(0.95f, 0.95f, 0.95f, 0.6f), HUDTextField);

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
    // Create object colors
    vector<vec4> obj_colors;
    for (int i = 0; i < num_vertices; i++) {
        obj_colors.push_back(color);
    }

    // Bind and load color buffers
    glBindBuffer(GL_ARRAY_BUFFER, ColorBuffers[buffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*colCoords*num_vertices, obj_colors.data(), GL_STATIC_DRAW);
}

void build_hud_quad(GLuint obj) {
    vector<vec2> vertices;
    vector<ivec3> indices;

    vertices = {
        {0, 0}, //0
        {1, 0}, //1
        {1, 1}, //2
        {0, 1} //3

    };

    indices = {
        {0, 1, 2},
        {0, 2, 3}

    };


    int numFaces = indices.size();
    numVertices[obj] = numFaces * 3;

    vector<vec4> obj_vertices;
    for (int i = 0; i < numFaces; i++) {
        for (int j = 0; j < 3; j++) {
            obj_vertices.push_back(vec4(vertices[indices[i][j]][0],
            vertices[indices[i][j]][1],
            0.0f,
            1.0f));
        }
    }

    //uvs for texture
    vector<vec2> uvs;
    uvs.push_back(vec2(0.0f, 0.0f));
    uvs.push_back(vec2(1.0f, 0.0f));
    uvs.push_back(vec2(1.0f, 1.0f));
    uvs.push_back(vec2(0.0f, 0.0f));
    uvs.push_back(vec2(1.0f, 1.0f));
    uvs.push_back(vec2(0.0f, 1.0f));


    glGenBuffers(NumObjBuffers, ObjBuffers[obj]);
    glBindVertexArray(VAOs[obj]);

    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][PosBuffer]);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(GLfloat) * posCoords * numVertices[obj],
                 obj_vertices.data(),
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][TexBuffer]);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(GLfloat) * texCoords * numVertices[obj],
                 uvs.data(),
                 GL_STATIC_DRAW);



}
