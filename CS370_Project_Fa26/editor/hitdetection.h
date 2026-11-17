#ifndef HITDETECTION_H
#define HITDETECTION_H

#include "ui.h"

extern void update_camera_matrices();
extern void render_scene();

enum GizmoAxis {GIZMO_NONE = 0, GIZMO_X = 1001, GIZMO_Y = 1002, GIZMO_Z = 1003 };
inline GizmoAxis activeGizmo = GIZMO_NONE;
inline int rawIndex = -1;


inline vec4 index_to_color(int i) {
    return {
        ((i & 0xFF)      ) / 255.0f,
        ((i >> 8) & 0xFF ) / 255.0f,
        ((i >> 16) & 0xFF) / 255.0f,
        1.0f
    };
}

inline int get_raw_index() {
    return rawIndex;
}

inline int color_to_index(unsigned char r, unsigned char g, unsigned char b) {
    return r | (g << 8) | (b << 16);
}

// Helper that draws one object with a raw color buffer ID (not an index into ColorBuffers)
inline void draw_picking_object(GLuint mesh, GLuint colorBuffer) {
    glUseProgram(colorProgram);
    glUniformMatrix4fv(colorProjMatLoc, 1, GL_FALSE, projMatrix);
    glUniformMatrix4fv(colorCamMatLoc, 1, GL_FALSE, cameraMatrix);
    glUniformMatrix4fv(colorModelMatLoc, 1, GL_FALSE, modelMatrix);

    glBindVertexArray(VAOs[mesh]);

    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[mesh][PosBuffer]);
    glVertexAttribPointer(colorVPos, posCoords, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(colorVPos);

    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glVertexAttribPointer(colorVCol, colCoords, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(colorVCol);

    glDrawArrays(GL_TRIANGLES, 0, numVertices[mesh]);
}


//function returns clicked scene object
inline SceneNode* pick_scene_object(const float mx, const float my) {
    update_camera_matrices();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLuint pickColorBuf;
    glGenBuffers(1, &pickColorBuf);

    // collect every enabled MeshNode that has a real mesh
    vector<MeshNode*> pickables;
    if (const Scene* scene = sceneManager.get_active()) {
        scene->for_each([&](SceneNode* n) {
            if (!n->enabled) return;
            if (auto* m = dynamic_cast<MeshNode*>(n)) {
                if (m->mesh != static_cast<GLuint>(-1))
                    pickables.push_back(m);
            }
        });
    }

    for (size_t i = 0; i < pickables.size(); ++i) {
        const MeshNode* meshNode = pickables[i];

        modelMatrix = meshNode->get_world_matrix();

        vec4 col = index_to_color(static_cast<int>(i) + 1);
        vector<vec4> colors(numVertices[meshNode->mesh], col);
        glBindBuffer(GL_ARRAY_BUFFER, pickColorBuf);
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(vec4) * numVertices[meshNode->mesh],
                     colors.data(),
                     GL_DYNAMIC_DRAW);

        draw_picking_object(meshNode->mesh, pickColorBuf);
    }

    // ---- gizmo picking (uses selected node) ----
    if (const SceneNode* selected = sceneManager.get_selected(); selected && gizmoArrowVAO != 0) {
        glDisable(GL_DEPTH_TEST);

        constexpr float size = 0.75f;
        constexpr float length = 1.0f;
        constexpr float offset = length * 2.0f;

        // world position of the selected node
        vec4 col = selected->get_world_matrix()[3];
        const mat4 trans = translate(vec3(col[0], col[1], col[2]));
        // or simply: mat4 trans = translate(selected->position);

        const mat4 transOffset = translate(0.0f, offset, 0.0f);
        const mat4 scale = vmath::scale(size, length, size);

        auto draw_pick_arrow = [&](const mat4& model, const int id) {
            modelMatrix = model;
            vec4 c = index_to_color(id);

            vector<vec4> colors(gizmoArrowNumVerts, c);
            glBindBuffer(GL_ARRAY_BUFFER, pickColorBuf);
            glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(vec4), colors.data(), GL_DYNAMIC_DRAW);

            glUseProgram(colorProgram);
            glUniformMatrix4fv(colorProjMatLoc,  1, GL_FALSE, projMatrix);
            glUniformMatrix4fv(colorCamMatLoc,   1, GL_FALSE, cameraMatrix);
            glUniformMatrix4fv(colorModelMatLoc, 1, GL_FALSE, modelMatrix);

            glBindVertexArray(gizmoArrowVAO);
            glBindBuffer(GL_ARRAY_BUFFER, gizmoArrowPosBuffer);
            glVertexAttribPointer(colorVPos, posCoords, GL_FLOAT, GL_FALSE, 0, nullptr);
            glEnableVertexAttribArray(colorVPos);

            glBindBuffer(GL_ARRAY_BUFFER, pickColorBuf);
            glVertexAttribPointer(colorVCol, colCoords, GL_FLOAT, GL_FALSE, 0, nullptr);
            glEnableVertexAttribArray(colorVCol);

            glDrawArrays(GL_TRIANGLES, 0, gizmoArrowNumVerts);
        };

        mat4 rotate = vmath::rotate(90.0f, vec3(0,0,1));
        draw_pick_arrow(trans * rotate * transOffset * scale, GIZMO_X);
        draw_pick_arrow(trans * transOffset * scale, GIZMO_Y);
        rotate = vmath::rotate(-90.0f, vec3(1,0,0));
        draw_pick_arrow(trans * rotate * transOffset * scale, GIZMO_Z);

        glEnable(GL_DEPTH_TEST);
    }

    // read pixel
    unsigned char pixel[4];
    glReadPixels(static_cast<GLint>(mx), static_cast<GLint>(my), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);

    const int index = color_to_index(pixel[0], pixel[1], pixel[2]);
    rawIndex = index;

    glDeleteBuffers(1, &pickColorBuf);

    // redraw real scene
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);
    render_scene();
    render_hud();
    glFlush();

    // map index back to node (gizmo IDs are > 1000, so they won't match)
    if (index > 0 && index <= static_cast<int>(pickables.size()))
        return pickables[index - 1];

    return nullptr;
}

#endif //HITDETECTION_H