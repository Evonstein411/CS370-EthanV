// Model vertex arrays and buffer objects
enum VAO_IDs {Square, Triangle, Trapezoid, Sun, NumVAOs};
GLuint VAOs[NumVAOs];
GLuint ObjBuffers[NumVAOs][NumObjBuffers];
GLint numVertices[NumVAOs];

// Color buffers
enum Color_Buffer_IDs {SkyBlue, GrassGreen, HouseBrown, RoofRed, FanBlue, SunYellow,
    MoonWhite, NightSky, SunsetSky, StoneBase, StoneSide, FanBase, NumColorBuffers,};
GLuint ColorBuffers[NumColorBuffers];

// Number of sun vertices
#define NUM_SUN 361

void build_square(GLuint obj);
void build_triangle(GLuint obj);
void build_trapezoid(GLuint obj);
void build_solid_color_buffer(GLuint num_vertices, vec4 color, GLuint buffer);
void build_gradient_color_buffer(vector<ivec3> indices, vector<vec4> colors, GLuint c_buff);
void build_sun(GLuint obj);

void build_geometry( )
{
    // Generate vertex arrays for objects
    glGenVertexArrays(NumVAOs, VAOs);

    // Generate color buffers
    glGenBuffers(NumColorBuffers, ColorBuffers);

    //build trap
    build_trapezoid(Trapezoid);
    build_solid_color_buffer(numVertices[Trapezoid], vec4(0.619, 0.004f, 0.004f,  1.0f), RoofRed);


    
    // Build square
    build_square(Square);
    
    // TODO: Build square solid color buffers
    build_solid_color_buffer(numVertices[Square], vec4(0.545f, 0.271f, 0.075f,  1.0f), HouseBrown);
    build_solid_color_buffer(numVertices[Square], vec4(0.5f, 0.5f, 0.5f,  1.0f), StoneBase);


    // Build triangle
    build_triangle(Triangle);
    
    // TODO: Build triangle solid color buffers
    build_solid_color_buffer(numVertices[Triangle], vec4(0.5f, 0.5f, 0.5f, 1.0f), StoneSide);


	// Build sun
	build_sun(Sun);
    build_solid_color_buffer(numVertices[Sun], vec4(1.0f, 0.9f, 0.9f, 1.0f), FanBase);
}

void build_square(GLuint obj) {
    vector<vec2> vertices;
    vector<ivec3> indices;

    // Bind square
    glBindVertexArray(VAOs[obj]);

    // TODO: Define square vertices
    vertices = {
            { 1.0f, 1.0f}, //0
            {-1.0f, 1.0f}, //1
            {-1.0f,-1.0f}, //2
            { 1.0f,-1.0f}, //3
    };

    // TODO: Define square face indices (ensure proper orientation)
    indices = {
        {0,1,3},
        {1,2,3},
    };
    int numFaces = indices.size();
    // Set numVertices as total number of INDICES (3*number of faces)
    numVertices[obj] = 3*numFaces;

    // Create gradient colors
    vector<vec4> blue_grad, dark_grad, green_grad, sunset_grad;
    // TODO: Define blue sky color
    blue_grad = {
        //red //gre //blu //alph
        {0.0f, 0.0f, 1.0f, 1.0f}, //0
        {0.0f, 0.0f, 1.0f, 1.0f}, //1
        {1.0f, 1.0f, 1.0f, 1.0f}, //2
        {1.0f, 1.0f, 1.0f, 1.0f}, //3
    };
    // TODO: Build sky gradient color buffer
    build_gradient_color_buffer(indices, blue_grad, SkyBlue);


    // TODO: Define green grass color
    green_grad = {
    //red //gre //blu //alph
    {0.0f, 1.0f, 0.0f, 1.0f}, //0
    {0.0f, 1.0f, 0.0f, 1.0f}, //1
    {0.0f, 0.5f, 0.0f, 1.0f}, //2
    {0.0f, 0.5f, 0.0f, 1.0f}, //3
    };
    // TODO: Build grass gradient color buffer
    build_gradient_color_buffer(indices, green_grad, GrassGreen);

    dark_grad = {
        {0.03f, 0.05f, 0.14f, 1.0f},
        {0.03f, 0.05f, 0.14f, 1.0f},
        {0.18f, 0.22f, 0.38f, 1.0f},
        {0.18f, 0.22f, 0.38f, 1.0f},
    };
    build_gradient_color_buffer(indices, dark_grad, NightSky);

    sunset_grad = {
        {0.20f, 0.10f, 0.35f, 1.0f},
        {0.20f, 0.10f, 0.35f, 1.0f},
        {0.95f, 0.45f, 0.20f, 1.0f},
        {0.95f, 0.45f, 0.20f, 1.0f},
    };
    build_gradient_color_buffer(indices, sunset_grad, SunsetSky);
    // TODO: Create object vertices from faces
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

    // TODO: Bind and load position object buffer for obj
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][PosBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*posCoords*numVertices[obj], obj_vertices.data(), GL_STATIC_DRAW);

}

void build_triangle(GLuint obj) {
    vector<vec2> vertices;
    vector<ivec3> indices;

    // Bind vertex array for obj
    glBindVertexArray(VAOs[obj]);

    // Define triangle vertices
    vertices = {
            { 1.0f, 1.0f}, //0
            {-1.0f, 1.0f}, //1
            {-1.0f,-1.0f}, //2
    };

    // TODO: Define triangle indices (ensure proper orientation)
    indices = {
        {0, 1, 2}
    };
    int numFaces = indices.size();
    // Set numVertices as total number of INDICES (3*number of faces)
    numVertices[obj] = 3*numFaces;

    // TODO: Define blue fan color
    vector<vec4> blue_grad = {
        //red //gre //blu //alph
        {0.0f, 0.01f, 1.0f, 1.0f}, //0
        {0.0f, 1.0f, 1.0f, 1.0f}, //1
        {0.0f, 0.01f, 1.0f, 1.0f}, //2
    };
    // TODO: Build fan gradient color buffer
    build_gradient_color_buffer(indices, blue_grad, FanBlue);


    // TODO: Create object vertices from faces
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

    // TODO: Bind and load position object buffer for obj
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][PosBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*posCoords*numVertices[obj], obj_vertices.data(), GL_STATIC_DRAW);

}

void build_trapezoid(GLuint obj) {
    vector<vec2> vertices;
    vector<ivec3> indices;

    glBindVertexArray(VAOs[obj]);

    vertices = {
        { 1.0f, -1.0f}, //0
        {0.5f, 1.0f}, //1
        {-0.5f,1.0f}, //2
        { -1.0f,-1.0f}, //3
    };

    indices = {
        {0,1,3},
        {1,2,3},
    };

    int numFaces = indices.size();
    // Set numVertices as total number of INDICES (3*number of faces)
    numVertices[obj] = 3*numFaces;

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

    // TODO: Bind and load position object buffer for obj
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][PosBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*posCoords*numVertices[obj], obj_vertices.data(), GL_STATIC_DRAW);

}




void build_sun(GLuint obj) {
    vector<vec4> vertices;
    vector<vec4> yellow_grad, gray_grad;

    // Bind vertex array for obj
    glBindVertexArray(VAOs[obj]);

    // TODO: Define sun vertices and colors

    //center
    vertices.push_back(vec4(0.0f, 0.0f, 0.0f, 1.0f));
    yellow_grad.push_back(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    gray_grad.push_back(vec4(1.0f, 1.0f, 1.0f, 1.0f));

    for(int i =0; i < NUM_SUN; i+= 1){
        float theta = i * (2.0f * M_PI) / (NUM_SUN - 2);
        vertices.push_back(vec4(cos(theta), sin(theta), 0.0f, 1.0f));
        yellow_grad.push_back(vec4(1.0f,0.75f,0.0f,1.0f));
        gray_grad.push_back(vec4(0.72f, 0.76f, 0.85f, 1.0f));

    }


	// Set numVertices
    numVertices[obj] = vertices.size();

    // Generate object buffers for obj
    glGenBuffers(NumObjBuffers, ObjBuffers[obj]);

    // TODO: Bind and load position object buffer for obj
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][PosBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*posCoords*numVertices[obj], vertices.data(), GL_STATIC_DRAW);

    // TODO: Bind and load color buffer
    glBindBuffer(GL_ARRAY_BUFFER, ColorBuffers[SunYellow]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * colCoords * numVertices[obj],yellow_grad.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, ColorBuffers[MoonWhite]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat) * colCoords * numVertices[obj],gray_grad.data(), GL_STATIC_DRAW);

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

