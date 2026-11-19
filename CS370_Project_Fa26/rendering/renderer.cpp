//
// Created by ethan on 11/18/2026.
//

#include "renderer.h"
#include "scene/nodes.h"
#include "shaders.h"
#include "geometry.h"
#include "drawobjects.h"
#include "editor/hitdetection.h"


void Renderer::initialize(int width, int height) {
    // Create geometry buffers
    build_geometry();
    build_materials();
    build_lights();
    build_textures();
    build_shaders();
    create_outline_fbo(width, height);

    //ui
    init_hud();

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
    draw_translation_gizmo(model, camera);
    //ui
    render_hud();

    glFlush();
}

void Renderer::draw_scene(const SceneModel& model) {
    if (const Scene* scene = model.get_active_scene())
        scene->draw();
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

    const auto* meshNode = dynamic_cast<const MeshNode*>(selected);
    if (!meshNode || meshNode->mesh == static_cast<GLuint>(-1)) return;

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

    glBindVertexArray(VAOs[meshNode->mesh]);
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[meshNode->mesh][PosBuffer]);
    glVertexAttribPointer(maskVPos, posCoords, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(maskVPos);
    glDrawArrays(GL_TRIANGLES, 0, numVertices[meshNode->mesh]);

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

void Renderer::draw_translation_gizmo(const SceneModel& model, const Camera& camera) {
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

    vmath::vec4 xColor = (activeGizmo == GIZMO_X) ? vmath::vec4(1.0f, 0.6f, 0.6f, 1.0f) : vmath::vec4(1.0f, 0.15f, 0.15f, 1.0f);
    vmath::vec4 yColor = (activeGizmo == GIZMO_Y) ? vmath::vec4(0.6f, 1.0f, 0.6f, 1.0f) : vmath::vec4(0.15f, 1.0f, 0.15f, 1.0f);
    vmath::vec4 zColor = (activeGizmo == GIZMO_Z) ? vmath::vec4(0.6f, 0.7f, 1.0f, 1.0f) : vmath::vec4(0.2f, 0.4f, 1.0f, 1.0f);

    const vmath::mat4& proj = camera.get_proj_matrix();
    const vmath::mat4& view = camera.get_view_matrix();

    // X
    vmath::mat4 rotate = vmath::rotate(90.0f, vmath::vec3(0, 0, 1));
    vmath::mat4 modelMatrix = trans * rotate * transOffset * scale;
    draw_gizmo_arrow(xColor, proj, view, modelMatrix);

    // Y
    modelMatrix = trans * transOffset * scale;
    draw_gizmo_arrow(yColor, proj, view, modelMatrix);

    // Z
    rotate = vmath::rotate(-90.0f, vmath::vec3(1, 0, 0));
    modelMatrix = trans * rotate * transOffset * scale;
    draw_gizmo_arrow(zColor, proj, view, modelMatrix);

    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
}
