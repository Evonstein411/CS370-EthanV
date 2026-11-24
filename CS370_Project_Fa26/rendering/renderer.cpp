//
// Created by ethan on 11/18/2026.
//

#include "renderer.h"
#include "../nodes/scenenode.h"
#include "shaders.h"
#include "geometry.h"
#include "drawobjects.h"
#include "globals.h"
#include "../nodes/meshcomponent.h"
#include "../nodes/lightcomponent.h"

Renderer::Renderer(SceneModel& model)
    : editorUI(model) {}

void Renderer::initialize(int width, int height) {
    // Create geometry buffers
    build_geometry();
    build_materials();
    build_lights();
    build_textures();
    build_shaders();
    create_outline_fbo(width, height);

    //ui
    HUD::instance().set_screen_size(width, height);
    editorUI.init();

    //open gl state
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);


}

void Renderer::resize(int width, int height) {
    create_outline_fbo(width, height);
}

void Renderer::render(SceneModel& model, int width, int height) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //get camera setup
    Camera& camera = model.get_active_camera();
    camera.update_matrices(width, height);

    //rebuild materials
    build_dynamic_materials();

    //objects/scene
    draw_scene(model);
    draw_selection_outline(model, camera, width, height);
    draw_translation_gizmo(model, camera, model.get_active_gizmo());
    //ui
    editorUI.update();// model -> fields
    HUD::instance().draw(); //draw widgets

    glFlush();
}

void Renderer::draw_scene(const SceneModel& model) {
    const Scene* scene = model.get_active_scene();
    if (!scene) return;
    const Camera camera = model.get_active_camera();

    scene->for_each([&](SceneNode* node) {
        if (!node || !node->enabled) return;
        if (auto* mesh = node->get_component<MeshComponent>()) {
            draw_mesh(mesh, node->get_world_matrix(), camera);
        }
        else if (auto* light = node->get_component<LightComponent>()) {
            // upload light or draw gizmo later
        }
    });

}

void Renderer::draw_mesh(const MeshComponent* mesh, const mat4& modelMatrix, Camera camera) {
    if (!mesh || mesh->mesh == static_cast<GLuint>(-1)) return;

    switch (mesh->appearance.mode) {
        case DrawMode::SolidColor:
            draw_color_object(mesh->mesh, mesh->appearance.colorBuffer, camera, modelMatrix);
            break;
        case DrawMode::Material:
            draw_mat_object(mesh->mesh, mesh->appearance.materialIndex, camera, modelMatrix);
            break;
        case DrawMode::Texture:
            draw_tex_object(mesh->mesh, mesh->appearance.texture, camera, modelMatrix);
            break;
        default:
            break;
    }
}

void Renderer::create_outline_fbo(const int width, const int height) {
    if (outlineFBO) {
        glDeleteFramebuffers(1, &outlineFBO);
        glDeleteTextures(1, &outlineMaskTex);
    }
    glGenFramebuffers(1, &outlineFBO);
    glGenTextures(1, &outlineMaskTex);

    glBindTexture(GL_TEXTURE_2D, outlineMaskTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, outlineFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outlineMaskTex, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::draw_selection_outline(const SceneModel& model, const Camera& camera, int width, int height) {
    SceneNode* selected = model.get_selected();
    if (!selected) return;
    if (!selected->get_component<MeshComponent>()) return;   // only outline meshes for now

    const auto* meshComponent = selected->get_component<MeshComponent>();
    if (!meshComponent || meshComponent->mesh == static_cast<GLuint>(-1)) return;

    GLuint mesh = meshComponent->mesh;
    vmath::mat4 modelMatrix = selected->get_world_matrix();

    // Mask pass
    glBindFramebuffer(GL_FRAMEBUFFER, outlineFBO);
    glViewport(0, 0, width, height);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    glUseProgram(maskProgram);
    glUniformMatrix4fv(maskProjMatLoc,  1, GL_FALSE, camera.get_proj_matrix());
    glUniformMatrix4fv(maskCamMatLoc,   1, GL_FALSE, camera.get_view_matrix());
    glUniformMatrix4fv(maskModelMatLoc, 1, GL_FALSE, modelMatrix);

    glBindVertexArray(VAOs[mesh]);
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[mesh][PosBuffer]);
    glVertexAttribPointer(maskVPos, posCoords, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(maskVPos);
    glDrawArrays(GL_TRIANGLES, 0, numVertices[mesh]);

    // Edge pass
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    glUseProgram(outlineProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, outlineMaskTex);
    glUniform2f(outlineTexelSizeLoc, 1.0f / width, 1.0f / height);
    glUniform4f(outlineColorLoc, 0.85f, 0.55f, 0.25f, 1.0f);
    glUniform1f(outlineThicknessLoc, 1.0f);

    glBindVertexArray(fullscreenVAO);

    glBindBuffer(GL_ARRAY_BUFFER, fullscreenPosBuffer);
    glVertexAttribPointer(outlineVPos, posCoords, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(outlineVPos);

    glBindBuffer(GL_ARRAY_BUFFER, fullscreenTexBuffer);
    glVertexAttribPointer(outlineVTex, texCoords, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(outlineVTex);

    glDrawArrays(GL_TRIANGLES, 0, fullscreenNumVertices);

    glEnable(GL_DEPTH_TEST);
}

void Renderer::draw_translation_gizmo(const SceneModel& model, const Camera& camera, GizmoAxis activeGizmo) {
    const SceneNode* selected = model.get_selected();
    if (!selected || gizmoArrowVAO == 0) return;

    constexpr float size = 0.35f;
    constexpr float length = 1.0f;
    constexpr float offset = length * 2.0f;

    vmath::vec4 col = selected->get_world_matrix()[3];
    vmath::mat4 trans = vmath::translate(vmath::vec3(col[0], col[1], col[2]));
    vmath::mat4 transOffset = vmath::translate(0.0f, offset, 0.0f);
    vmath::mat4 scale = vmath::scale(size, length, size);

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    vmath::vec4 xColor = (activeGizmo == GizmoAxis::X) ? vmath::vec4(1.0f, 0.6f, 0.6f, 1.0f) : vmath::vec4(1.0f, 0.15f, 0.15f, 1.0f);
    vmath::vec4 yColor = (activeGizmo == GizmoAxis::Y) ? vmath::vec4(0.6f, 1.0f, 0.6f, 1.0f) : vmath::vec4(0.15f, 1.0f, 0.15f, 1.0f);
    vmath::vec4 zColor = (activeGizmo == GizmoAxis::Z) ? vmath::vec4(0.6f, 0.7f, 1.0f, 1.0f) : vmath::vec4(0.2f, 0.4f, 1.0f, 1.0f);

    // X
    vmath::mat4 rotate = vmath::rotate(90.0f, vmath::vec3(0, 0, 1));
    vmath::mat4 modelMatrix = trans * rotate * transOffset * scale;
    draw_gizmo_arrow(xColor, camera, modelMatrix);

    // Y
    modelMatrix = trans * transOffset * scale;
    draw_gizmo_arrow(yColor, camera, modelMatrix);

    // Z
    rotate = vmath::rotate(-90.0f, vmath::vec3(1, 0, 0));
    modelMatrix = trans * rotate * transOffset * scale;
    draw_gizmo_arrow(zColor, camera, modelMatrix);

    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
}

HitResult Renderer::pick(SceneModel &model, float mx, float my) {
    HitResult result;

    const Camera& cam = model.get_active_camera();
    vmath::mat4 proj = cam.get_proj_matrix();
    vmath::mat4 view = cam.get_view_matrix();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLuint pickColorBuf;
    glGenBuffers(1, &pickColorBuf);

    std::vector<SceneNode*> pickables;
    if (const Scene* scene = model.get_active_scene()) {
        scene->for_each([&](SceneNode* n) {
            if (!n || !n->enabled) return;
            if (auto* mesh = n->get_component<MeshComponent>()) {
                if (mesh->mesh != static_cast<GLuint>(-1))
                    pickables.push_back(n);
            }
        });
    }

    printf("colorProgram: %u\n", colorProgram);
    glUseProgram(colorProgram);
    glUniformMatrix4fv(colorProjMatLoc, 1, GL_FALSE, proj);
    glUniformMatrix4fv(colorCamMatLoc, 1, GL_FALSE, view);

    for (size_t i = 0; i < pickables.size(); ++i) {
        SceneNode* node = pickables[i];
        auto* meshComp = node->get_component<MeshComponent>();

        vmath::mat4 modelMat = node->get_world_matrix();
        glUniformMatrix4fv(colorModelMatLoc, 1, GL_FALSE, modelMat);

        vmath::vec4 col = index_to_color(static_cast<int>(i) + 1);
        std::vector<vmath::vec4> colors(numVertices[meshComp->mesh], col);

        glBindBuffer(GL_ARRAY_BUFFER, pickColorBuf);
        glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(vmath::vec4),
                     colors.data(), GL_DYNAMIC_DRAW);


        draw_picking_object(meshComp->mesh, pickColorBuf, proj, view, modelMat);
    }

    // gizmo pass (same logic as the old hitdetection, using local proj/view)
    if (SceneNode* selected = model.get_selected(); selected && gizmoArrowVAO != 0) {
        glDisable(GL_DEPTH_TEST);

        constexpr float size = 0.75f;
        constexpr float length = 1.0f;
        constexpr float offset = length * 2.0f;

        // world position of the selected node
        vec4 col = selected->get_world_matrix()[3];
        const mat4 trans = translate(vec3(col[0], col[1], col[2]));
        const mat4 transOffset = translate(0.0f, offset, 0.0f);
        const mat4 scale = vmath::scale(size, length, size);

        mat4 rotate = vmath::rotate(90.0f, vec3(0,0,1));
        draw_pick_arrow(pickColorBuf, proj, view, trans * rotate * transOffset * scale, GizmoAxis::X);
        draw_pick_arrow(pickColorBuf, proj, view, trans * transOffset * scale, GizmoAxis::Y);
        rotate = vmath::rotate(-90.0f, vec3(1,0,0));
        draw_pick_arrow(pickColorBuf, proj, view, trans * rotate * transOffset * scale, GizmoAxis::Z);
    }

    unsigned char pixel[4];
    glReadPixels(static_cast<GLint>(mx), static_cast<GLint>(my), 1, 1,
                 GL_RGBA, GL_UNSIGNED_BYTE, pixel);

    int index = color_to_index(pixel[0], pixel[1], pixel[2]);
    glDeleteBuffers(1, &pickColorBuf);

    if (index == static_cast<int>(GizmoAxis::X) ||
        index == static_cast<int>(GizmoAxis::Y) ||
        index == static_cast<int>(GizmoAxis::Z)) {
        result.gizmo = static_cast<GizmoAxis>(index);
        result.node = model.get_selected();
    } else if (index > 0 && index <= static_cast<int>(pickables.size())) {
        result.node = pickables[index - 1];
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);
    render(model, HUD::instance().get_screen_width(), HUD::instance().get_screen_height());
    return result;
}

void Renderer::draw_picking_object(GLuint mesh, GLuint colorBuffer, mat4 projMatrix, mat4 cameraMatrix, mat4 modelMatrix) {
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

void Renderer::draw_pick_arrow(GLuint pickColorBuf, mat4 projMatrix, mat4 cameraMatrix, mat4 modelMatrix, GizmoAxis id) {
    vec4 c = index_to_color(static_cast<int>(id));

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


vmath::vec4 Renderer::index_to_color(int i) {
    return {
        ((i & 0xFF)      ) / 255.0f,
        ((i >> 8) & 0xFF ) / 255.0f,
        ((i >> 16) & 0xFF) / 255.0f,
        1.0f
    };
}

int Renderer::color_to_index(unsigned char r, unsigned char g, unsigned char b) {
    return r | (g << 8) | (b << 16);
}