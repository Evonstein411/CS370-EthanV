#include <string>
#include <filesystem>
#include <array>
#include <cstdio>

namespace fs = std::filesystem;
// Model vertex arrays and buffer objects
vector<string> meshNames;
vector<GLuint> VAOs;
vector<array<GLuint, NumObjBuffers>> ObjBuffers;
vector<GLint> numVertices;

//HUD should be migrated at some point to kitUI
enum HUD_IDs { HUDQuad = 0, HUDTextQuad, NumHUD };
GLuint HUD_VAOs[NumHUD];
GLuint HUD_ObjBuffers[NumHUD][NumObjBuffers];
GLint HUD_numVertices[NumHUD];

// Color buffers
vector<GLuint> ColorBuffers;

float color_r = 0.0f;
float color_g = 0.0f;
float color_b = 0.0f;
float color_a = 0.0f;


// Model files
const char * cubeFile = "../../common/models/unitcube.obj";
const char * sphereFile = "../../common/models/sphere.obj";



int load_model(const char* filename, const string& displayName);
void build_solid_color_buffer(GLuint num_vertices, vec4 color, GLuint buffer);
void build_hud_quad(GLuint hudObj);
void scan_models(const string& folder = "../models");
int add_model_file(const string& path);


int find_mesh(const string& name) {
    for (int i = 0; i < (int)meshNames.size(); ++i)
        if (meshNames[i] == name) return i;
    return -1;
}

void build_geometry() {
    // HUD quads (unchanged logic, just using the HUD_* arrays)
    glGenVertexArrays(NumHUD, HUD_VAOs);
    build_hud_quad(HUDQuad);
    build_hud_quad(HUDTextQuad);

    // Selectable models from folder
    scan_models();
}


void scan_models(const string& folder) {
    meshNames.clear();
    VAOs.clear();
    ObjBuffers.clear();
    numVertices.clear();

    if (!fs::exists(folder) || !fs::is_directory(folder))
        return;

    for (const auto& entry : fs::directory_iterator(folder)) {
        if (!entry.is_regular_file()) continue;
        fs::path p = entry.path();
        string ext = p.extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        if (ext != ".obj") continue;

        load_model(p.string().c_str(), p.stem().string());
    }
}

int add_model_file(const string& path) {
    if (path.empty() || !fs::exists(path)) return -1;

    fs::path src(path);
    fs::path destDir = "../models";
    if (!fs::exists(destDir))
        fs::create_directories(destDir);

    fs::path dest = destDir / src.filename();
    if (fs::exists(dest)) {
        int n = 1;
        fs::path candidate;
        do {
            candidate = destDir / (src.stem().string() + "_" + std::to_string(n) + src.extension().string());
            n++;
        } while (fs::exists(candidate));
        dest = candidate;
    }

    std::error_code ec;
    fs::copy_file(src, dest, ec);
    if (ec) return -1;

    return load_model(dest.string().c_str(), dest.stem().string());
}


int load_model(const char * filename, const string& displayName) {
    vector<vec4> vertices;
    vector<vec2> uvCoords;
    vector<vec3> normals;

    // Load model and set number of vertices
    if (!loadOBJ(filename, vertices, uvCoords, normals) || vertices.empty()) {
        fprintf(stderr, "ERROR: failed to load model %s\n", filename);
        return -1;
    }

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    array<GLuint, NumObjBuffers> bufs{};
    glGenBuffers(NumObjBuffers, bufs.data());

    glBindBuffer(GL_ARRAY_BUFFER, bufs[PosBuffer]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat) * posCoords * vertices.size(),vertices.data(),GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, bufs[NormBuffer]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat) * normCoords * normals.size(),normals.data(),GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, bufs[TexBuffer]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat) * texCoords * uvCoords.size(),uvCoords.data(),GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    VAOs.push_back(vao);
    ObjBuffers.push_back(bufs);
    numVertices.push_back((GLint)vertices.size());
    meshNames.push_back(displayName);

    return (int)meshNames.size() - 1;   // index of the new mesh


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
    HUD_numVertices[obj] = numFaces * 3;

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


    glGenBuffers(NumObjBuffers, HUD_ObjBuffers[obj]);
    glBindVertexArray(HUD_VAOs[obj]);

    glBindBuffer(GL_ARRAY_BUFFER, HUD_ObjBuffers[obj][PosBuffer]);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(GLfloat) * posCoords * HUD_numVertices[obj],
                 obj_vertices.data(),
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, HUD_ObjBuffers[obj][TexBuffer]);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(GLfloat) * texCoords * HUD_numVertices[obj],
                 uvs.data(),
                 GL_STATIC_DRAW);

}
