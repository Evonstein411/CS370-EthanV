#ifndef DRAW_OBJECTS_H
#define DRAW_OBJECTS_H
#include "shaders.h"
#include "lights.h"
#include "materials.h"
#include "textures.h"
#include "geometry.h"
#include <vector>


inline GLuint dynamicHUDColorBuffer = 0;
inline GLuint highlightColorBuffer = 0;


inline void draw_color_object(const GLuint obj, const GLuint color, Camera camera, vmath::mat4 modelMatrix) {
    vmath::mat4 projMatrix = camera.get_proj_matrix();
    vmath::mat4 cameraMatrix = camera.get_view_matrix();

    // Select shader program
    glUseProgram(colorProgram);

    // Pass projection matrix to shader
    glUniformMatrix4fv(colorProjMatLoc, 1, GL_FALSE, projMatrix);

    // Pass camera matrix to shader
    glUniformMatrix4fv(colorCamMatLoc, 1, GL_FALSE, cameraMatrix);

    // Pass model matrix to shader
    glUniformMatrix4fv(colorModelMatLoc, 1, GL_FALSE, modelMatrix);

    // Bind vertex array
    glBindVertexArray(VAOs[obj]);

    // Bind position object buffer and set attributes
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][PosBuffer]);
    glVertexAttribPointer(colorVPos, posCoords, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(colorVPos);

    // Bind color buffer and set attributes
    glBindBuffer(GL_ARRAY_BUFFER, ColorBuffers[color]);
    glVertexAttribPointer(colorVCol, colCoords, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(colorVCol);

    // Draw geometry
    glDrawArrays(GL_TRIANGLES, 0, numVertices[obj]);
}

inline void draw_mat_object(const GLuint obj, const GLuint material, Camera camera, vmath::mat4 modelMatrix) {
    vmath::mat4 projMatrix = camera.get_proj_matrix();
    vmath::mat4 cameraMatrix = camera.get_view_matrix();
    vmath::mat4 normalMatrix = modelMatrix.inverse().transpose();

    // Select shader program
    glUseProgram(lightingProgram);

    // Pass projection and camera matrices to shader
    glUniformMatrix4fv(lightingProjMatLoc, 1, GL_FALSE, projMatrix);
    glUniformMatrix4fv(lightingCameraMatLoc, 1, GL_FALSE, cameraMatrix);

    // Bind lights
    glUniformBlockBinding(lightingProgram, lightingLightsBlockIdx, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, LightBuffers[LightBuffer], 0, lights.size()*sizeof(LightProperties));

    // Bind materials
    glUniformBlockBinding(lightingProgram, lightingMaterialsBlockIdx, 1);
    size_t total = materials.size() + dynamicMaterials.size();
    glBindBufferRange(GL_UNIFORM_BUFFER, 1, MaterialBuffers[MaterialBuffer], 0,
                      total * sizeof(Material));
    // Set camera position
    glUniform3fv(lightingEyeLoc, 1, camera.get_eye());

    // Set num lights and lightOn
    glUniform1i(lightingNumLightsLoc, lights.size());
    glUniform1iv(lightingLightOnLoc, NumLights, lightOn);

    // Pass model matrix and normal matrix to shader
    glUniformMatrix4fv(lightingModelMatLoc, 1, GL_FALSE, modelMatrix);
    glUniformMatrix4fv(lightingNormMatLoc, 1, GL_FALSE, normalMatrix);

    // Pass material index to shader
    glUniform1i(lightingMaterialLoc, material);

    // Bind vertex array
    glBindVertexArray(VAOs[obj]);

    // Bind position object buffer and set attributes
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][PosBuffer]);
    glVertexAttribPointer(lightingVPos, posCoords, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(lightingVPos);

    // Bind normal object buffer and set attributes
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][NormBuffer]);
    glVertexAttribPointer(lightingVNorm, normCoords, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(lightingVNorm);

    // Draw object
    glDrawArrays(GL_TRIANGLES, 0, numVertices[obj]);
}

inline void draw_gizmo_arrow(const vmath::vec4& color, Camera camera, const vmath::mat4& model) {
    vmath::mat4 proj = camera.get_proj_matrix();
    vmath::mat4 view = camera.get_view_matrix();
    if (gizmoArrowVAO == 0) return;

    static GLuint gizmoColorBuffer = 0;
    if (gizmoColorBuffer == 0) glGenBuffers(1, &gizmoColorBuffer);

    std::vector<vmath::vec4> cols(gizmoArrowNumVerts, color);
    glBindBuffer(GL_ARRAY_BUFFER, gizmoColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, cols.size() * sizeof(vmath::vec4), cols.data(), GL_DYNAMIC_DRAW);

    glUseProgram(colorProgram);
    glUniformMatrix4fv(colorProjMatLoc,  1, GL_FALSE, proj);
    glUniformMatrix4fv(colorCamMatLoc,   1, GL_FALSE, view);
    glUniformMatrix4fv(colorModelMatLoc, 1, GL_FALSE, model);

    glBindVertexArray(gizmoArrowVAO);

    glBindBuffer(GL_ARRAY_BUFFER, gizmoArrowPosBuffer);
    glVertexAttribPointer(colorVPos, posCoords, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(colorVPos);

    glBindBuffer(GL_ARRAY_BUFFER, gizmoColorBuffer);
    glVertexAttribPointer(colorVCol, colCoords, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(colorVCol);

    glDrawArrays(GL_TRIANGLES, 0, gizmoArrowNumVerts);
}


inline void draw_tex_object(const GLuint obj, const GLuint textureID, Camera camera, vmath::mat4 modelMatrix){
    vmath::mat4 projMatrix = camera.get_proj_matrix();
    vmath::mat4 cameraMatrix = camera.get_view_matrix();
    // Select shader program
    glUseProgram(textureProgram);

    // Pass projection matrix to shader
    glUniformMatrix4fv(textureProjMatLoc, 1, GL_FALSE, projMatrix);

    // Pass camera matrix to shader
    glUniformMatrix4fv(textureCameraMatLoc, 1, GL_FALSE, cameraMatrix);

    // Pass model matrix to shader
    glUniformMatrix4fv(textureModelMatLoc, 1, GL_FALSE, modelMatrix);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Bind vertex array
    glBindVertexArray(VAOs[obj]);

    // Bind position object buffer and set attributes
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][PosBuffer]);
    glVertexAttribPointer(textureVPos, posCoords, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(textureVPos);

    // Bind texture object buffer and set attributes
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][TexBuffer]);
    glVertexAttribPointer(textureVTex, texCoords, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(textureVTex);

    // Draw object
    glDrawArrays(GL_TRIANGLES, 0, numVertices[obj]);
}






#endif // DRAW_OBJECTS_H