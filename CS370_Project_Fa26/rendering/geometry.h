#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <string>
#include <filesystem>
#include <array>
#include <cstdio>
#include "../common/objloader.h"

namespace fs = std::filesystem;
// Model vertex arrays and buffer objects
inline vector<string> meshNames;
inline vector<GLuint> VAOs;
inline vector<array<GLuint, NumObjBuffers>> ObjBuffers;
inline vector<GLint> numVertices;

//Mask
// Dedicated NDC full-screen quad for post-process passes (outline, etc.)
inline GLuint fullscreenVAO = 0;
inline GLuint fullscreenPosBuffer = 0;
inline GLuint fullscreenTexBuffer = 0;
inline GLint  fullscreenNumVertices = 6;

//Gizmo
inline GLuint gizmoArrowVAO = 0;
inline GLuint gizmoArrowPosBuffer = 0;
inline GLuint gizmoArrowNormBuffer = 0;
inline GLint  gizmoArrowNumVerts = 0;

//HUD should be migrated at some point to kitUI
enum HUD_IDs { HUDQuad = 0, HUDTextQuad, NumHUD };

inline GLuint HUD_VAOs[NumHUD];
inline GLuint HUD_ObjBuffers[NumHUD][NumObjBuffers];
inline GLint HUD_numVertices[NumHUD];

// Color buffers
inline vector<GLuint> ColorBuffers;

inline float colorR = 0.0f;
inline float colorG = 0.0f;
inline float colorB = 0.0f;
inline float colorA = 0.0f;


// Model files
inline auto cubeFile = "../../common/models/unitcube.obj";
inline auto sphereFile = "../../common/models/sphere.obj";



int load_model(const char* filename, const string& displayName);
void load_gizmo_arrow();
void build_solid_color_buffer(GLuint numVertices, const vec4& color, GLuint buffer);
void build_hud_quad(GLuint obj);
void scan_models(const string& folder = "../assets/models");
int add_model_file(const string& path);
void build_fullscreen_quad();

inline int find_mesh(const string& name) {
    for (int i = 0; i < static_cast<int>(meshNames.size()); ++i)
        if (meshNames[i] == name) return i;
    return -1;
}

inline void build_geometry() {
    // HUD quads (unchanged logic, just using the HUD_* arrays)
    glGenVertexArrays(NumHUD, HUD_VAOs);
    build_hud_quad(HUDQuad);
    build_hud_quad(HUDTextQuad);
    build_fullscreen_quad();
    load_gizmo_arrow();
    // Selectable models from folder
    scan_models();
}


inline void scan_models(const string& folder) {
    meshNames.clear();
    VAOs.clear();
    ObjBuffers.clear();
    numVertices.clear();

    if (!fs::exists(folder) || !fs::is_directory(folder))
        return;

    for (const auto& entry : fs::directory_iterator(folder)) {
        if (!entry.is_regular_file()) continue;
        const fs::path& p = entry.path();
        string ext = p.extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        if (ext != ".obj") continue;

        load_model(p.string().c_str(), p.stem().string());
    }
}

inline int add_model_file(const string& path) {
    if (path.empty() || !fs::exists(path)) return -1;

    const fs::path src(path);
    const fs::path destDir = "../assets/models";
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


inline int load_model(const char * filename, const string& displayName) {
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
    numVertices.push_back(static_cast<GLint>(vertices.size()));
    meshNames.push_back(displayName);

    return static_cast<int>(meshNames.size()) - 1;   // index of the new mesh


}

inline void build_solid_color_buffer(GLuint numVertices, const vec4& color, const GLuint buffer) {
    // Create object colors
    vector<vec4> obj_colors;
    obj_colors.reserve(numVertices);
    for (int i = 0; i < numVertices; i++) {
        obj_colors.push_back(color);
    }

    // Bind and load color buffers
    glBindBuffer(GL_ARRAY_BUFFER, ColorBuffers[buffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*colCoords*numVertices, obj_colors.data(), GL_STATIC_DRAW);
}

inline void build_hud_quad(const GLuint obj) {
    vector<vec2> vertices = {
        {0, 0}, //0
        {1, 0}, //1
        {1, 1}, //2
        {0, 1} //3

    };

    vector<ivec3> indices = {
        {0, 1, 2},
        {0, 2, 3}

    };


    const int numFaces = indices.size();
    HUD_numVertices[obj] = numFaces * 3;

    vector<vec4> obj_vertices;
    for (int i = 0; i < numFaces; i++) {
        for (int j = 0; j < 3; j++) {
            obj_vertices.emplace_back(vertices[indices[i][j]][0],
            vertices[indices[i][j]][1],
            0.0f,
            1.0f);
        }
    }

    //uvs for texture
    vector<vec2> uvs;
    uvs.emplace_back(0.0f, 0.0f);
    uvs.emplace_back(1.0f, 0.0f);
    uvs.emplace_back(1.0f, 1.0f);
    uvs.emplace_back(0.0f, 0.0f);
    uvs.emplace_back(1.0f, 1.0f);
    uvs.emplace_back(0.0f, 1.0f);


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


inline void build_fullscreen_quad() {
    // NDC positions (x, y, z, w) + UVs
    // Two triangles covering the entire clip space
    const vector<vec4> positions = {
        vec4(-1.0f, -1.0f, 0.0f, 1.0f),
        vec4( 1.0f, -1.0f, 0.0f, 1.0f),
        vec4( 1.0f,  1.0f, 0.0f, 1.0f),

        vec4(-1.0f, -1.0f, 0.0f, 1.0f),
        vec4( 1.0f,  1.0f, 0.0f, 1.0f),
        vec4(-1.0f,  1.0f, 0.0f, 1.0f)
    };

    const vector<vec2> uvs = {
        vec2(0.0f, 0.0f),
        vec2(1.0f, 0.0f),
        vec2(1.0f, 1.0f),

        vec2(0.0f, 0.0f),
        vec2(1.0f, 1.0f),
        vec2(0.0f, 1.0f)
    };

    glGenVertexArrays(1, &fullscreenVAO);
    glBindVertexArray(fullscreenVAO);

    glGenBuffers(1, &fullscreenPosBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, fullscreenPosBuffer);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(vec4), positions.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &fullscreenTexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, fullscreenTexBuffer);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vec2), uvs.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

inline void load_gizmo_arrow() {
    vector<vec4> vertices;
    vector<vec2> uvs;
    vector<vec3> normals;

    if (!loadOBJ("../assets/models/arrow.obj", vertices, uvs, normals) || vertices.empty()) {
        fprintf(stderr, "Failed to load gizmo arrow.obj\n");
        return;
    }

    gizmoArrowNumVerts = static_cast<GLint>(vertices.size());

    glGenVertexArrays(1, &gizmoArrowVAO);
    glBindVertexArray(gizmoArrowVAO);

    glGenBuffers(1, &gizmoArrowPosBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, gizmoArrowPosBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec4), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &gizmoArrowNormBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, gizmoArrowNormBuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), normals.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

#endif //GEOMETRY_H