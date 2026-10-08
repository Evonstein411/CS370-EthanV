// Model vertex arrays and buffer objects
enum VAO_IDs {Cube, Sphere, Pyramid, Target, NumVAOs};
GLuint VAOs[NumVAOs];
GLuint ObjBuffers[NumVAOs][NumObjBuffers];
GLint numVertices[NumVAOs];

// Color buffers
enum Color_Buffer_IDs {PyramidGrad, SphereMagenta, BarBlack, TableColor, TargetColor,  NumColorBuffers};
GLuint ColorBuffers[NumColorBuffers];

// Model files
const char * cubeFile = "../../common/models/unitcube.obj";
const char * sphereFile = "../../common/models/sphere.obj";

// Axis length
GLfloat axis_length = 3.0f;

void build_pyramid(GLuint p_obj, GLuint p_buff);
void build_target(GLuint obj, GLuint t_buff);
void build_solid_color_buffer(GLuint num_vertices, vec4 color, GLuint buffer);
void build_gradient_color_buffer(vector<ivec3> indices, vector<vec4> colors, GLuint c_buff);
void load_model(const char * filename, GLuint obj);

void build_geometry( )
{
    // Generate vertex arrays for objects
    glGenVertexArrays(NumVAOs, VAOs);
    // Generate color buffers
    glGenBuffers(NumColorBuffers, ColorBuffers);

    /******************************************/
    /*       INSERT (e) CODE HERE          */
    /******************************************/
    // TODO: Load sphere model
    load_model(sphereFile, Sphere);

    // TODO: Build SphereMagenta color buffer
    build_solid_color_buffer(numVertices[Sphere], {1.0f, 0.0f, 1.0f, 1.0f}, SphereMagenta);



    // Load models
    load_model(cubeFile, Cube);

    // Build target and pyramid geometries
    build_target(Target, TargetColor);
    build_pyramid(Pyramid, PyramidGrad);

    // Build table and bar color buffers
    build_solid_color_buffer(numVertices[Cube], vec4(1.0f, 1.0f, 1.0f, 1.0f), TableColor);
    build_solid_color_buffer(numVertices[Cube], vec4(0.0f, 0.0f, 0.0f, 1.0f), BarBlack);


}

void build_pyramid(GLuint p_obj, GLuint p_buff) {
    vector<vec4> vertices;
    vector<vec4> colors;
    vector<ivec3> indices;

    // Bind pyramid vertex array object
    glBindVertexArray(VAOs[p_obj]);

    // Define vertices - DO NOT MODIFY
    vertices = {
            {0.5f, 1.0f, 0.5f, 1.0f}, //0
            {0.0f, 0.0f, 0.0f, 1.0f}, //1
            {1.0f, 0.0f, 0.0f, 1.0f}, //2
            {1.0f, 0.0f, 1.0f, 1.0f},//3
            {0.0f, 0.0f, 1.0f, 1.0f}//4
    };

    /******************************************/
    /*       INSERT (c) CODE HERE             */
    /******************************************/
    // TODO: Define pyramid face indices (ensure proper orientation)
    indices = {
        {1,0,2},  
        {2,0,3},
        {3,0,4},
        {4,0,1},
        {1,2,3},
        {1,3,4}
    };
    int numFaces = indices.size();

    // TODO: Define face colors (per vertex)
    colors = {
        //r g b o
        {1.0f, 1.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f, 1.0f},
        {0.0f, 0.0f, 1.0f, 1.0f},
        {1.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f, 1.0f},
    };

    // TODO: Build gradient color buffer
    build_gradient_color_buffer(indices, colors, p_buff);



    // ******************************
    // DO NOT MODIFY BELOW THIS POINT
    // ******************************
    // Create object vertices and colors from faces
    vector<vec4> obj_vertices;
    for (int i = 0; i < numFaces; i++) {
        for (int j = 0; j < 3; j++) {
            obj_vertices.push_back(vertices[indices[i][j]]);
        }
    }

    // Set numVertices as total number of INDICES
    numVertices[p_obj] = 3*numFaces;

    // Generate object buffers for Pyramid
    glGenBuffers(NumObjBuffers, ObjBuffers[p_obj]);

    // Bind pyramid positions
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[p_obj][PosBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*posCoords*numVertices[p_obj], obj_vertices.data(), GL_STATIC_DRAW);
}

void build_solid_color_buffer(GLuint num_vertices, vec4 color, GLuint buffer) {
    vector<vec4> obj_colors;
    for (int i = 0; i < num_vertices; i++) {
        obj_colors.push_back(color);
    }

    glBindBuffer(GL_ARRAY_BUFFER, ColorBuffers[buffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*colCoords*num_vertices, obj_colors.data(), GL_STATIC_DRAW);
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

void build_target(GLuint t_obj, GLuint t_buff) {
    vector<vec4> vertices;
    vector<ivec3> indices;
    vector<vec4> colors;

    // Bind target vertex array object
    glBindVertexArray(VAOs[t_obj]);

    // Define vertices for table boundary
    vertices = {
            {-TABLE_SIZE/6, 0.02f, 0.0f, 1.0f},
            {TABLE_SIZE/6, 0.02f, 0.0f, 1.0f},
            {0.0f, 0.02f, TABLE_SIZE/6, 1.0f},
            {0.0f, 0.02f,-TABLE_SIZE/6, 1.0f}
    };

    // Define vertices for table circle
    for (int i = 4; i < 68; i++) {
        vertices.push_back(vec4(CIRCLE_RAD*sin((i-4)*0.1f), 0.02f, CIRCLE_RAD*cos((i-4)*0.1f), 1.0f));
    }

    // Define axis colors (red - x, blue - z)
    colors.push_back(vec4(1.0f, 0.0f, 0.0f, 1.0f));
    colors.push_back(vec4(1.0f, 0.0f, 0.0f, 1.0f));
    colors.push_back(vec4(0.0f, 0.0f, 1.0f, 1.0f));
    colors.push_back(vec4(0.0f, 0.0f, 1.0f, 1.0f));

    // Define circle colors (black)
    for (int i = 4; i < 68; i++) {
        colors.push_back(vec4(0.0f, 0.0f, 0.0f, 1.0f));
    }

    // Set numVertices
    numVertices[t_obj] = vertices.size();

    // Generate object buffer for table
    glGenBuffers(NumObjBuffers, ObjBuffers[t_obj]);

    // Bind target positions
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[t_obj][PosBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*posCoords*numVertices[t_obj], vertices.data(), GL_STATIC_DRAW);

    // Bind target colors
    glBindBuffer(GL_ARRAY_BUFFER, ColorBuffers[t_buff]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*colCoords*numVertices[t_obj], colors.data(), GL_STATIC_DRAW);
}
