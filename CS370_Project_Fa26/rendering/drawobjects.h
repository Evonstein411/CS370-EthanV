#ifndef DRAW_OBJECTS_H
#define DRAW_OBJECTS_H
#include "shaders.h"

GLuint dynamicHUDColorBuffer = 0;
GLuint highlightColorBuffer = 0;


void draw_color_object(GLuint obj, GLuint color) {
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
    glVertexAttribPointer(colorVPos, posCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(colorVPos);

    // Bind color buffer and set attributes
    glBindBuffer(GL_ARRAY_BUFFER, ColorBuffers[color]);
    glVertexAttribPointer(colorVCol, colCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(colorVCol);

    // Draw geometry
    glDrawArrays(GL_TRIANGLES, 0, numVertices[obj]);
}

void draw_mat_object(GLuint obj, GLuint material){
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
                      total * sizeof(MaterialProperties));
    // Set camera position
    glUniform3fv(lightingEyeLoc, 1, eye);

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
    glVertexAttribPointer(lightingVPos, posCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(lightingVPos);

    // Bind normal object buffer and set attributes
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][NormBuffer]);
    glVertexAttribPointer(lightingVNorm, normCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(lightingVNorm);

    // Draw object
    glDrawArrays(GL_TRIANGLES, 0, numVertices[obj]);
}

void draw_gizmo_arrow(const vec4& color) {
    if (gizmoArrowVAO == 0) return;

    // Re-use your existing highlightColorBuffer or make a tiny dedicated one
    static GLuint gizmoColorBuffer = 0;
    if (gizmoColorBuffer == 0) glGenBuffers(1, &gizmoColorBuffer);

    vector<vec4> cols(gizmoArrowNumVerts, color);
    glBindBuffer(GL_ARRAY_BUFFER, gizmoColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, cols.size() * sizeof(vec4), cols.data(), GL_DYNAMIC_DRAW);

    glUseProgram(colorProgram);
    glUniformMatrix4fv(colorProjMatLoc,  1, GL_FALSE, projMatrix);
    glUniformMatrix4fv(colorCamMatLoc,   1, GL_FALSE, cameraMatrix);
    glUniformMatrix4fv(colorModelMatLoc, 1, GL_FALSE, modelMatrix);

    glBindVertexArray(gizmoArrowVAO);

    glBindBuffer(GL_ARRAY_BUFFER, gizmoArrowPosBuffer);
    glVertexAttribPointer(colorVPos, posCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(colorVPos);

    glBindBuffer(GL_ARRAY_BUFFER, gizmoColorBuffer);
    glVertexAttribPointer(colorVCol, colCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(colorVCol);

    glDrawArrays(GL_TRIANGLES, 0, gizmoArrowNumVerts);
}



void draw_tex_object(GLuint obj, GLuint textureID){
    // Select shader program
    glUseProgram(textureProgram);

    // Pass projection matrix to shader
    glUniformMatrix4fv(textureProjMatLoc, 1, GL_FALSE, projMatrix);

    // Pass camera matrix to shader
    glUniformMatrix4fv(textureCameraMatLoc, 1, GL_FALSE, cameraMatrix);

    // Pass model matrix to shader
    glUniformMatrix4fv(textureModelMatLoc, 1, GL_FALSE, modelMatrix);

    // TODO: Bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Bind vertex array
    glBindVertexArray(VAOs[obj]);

    // Bind position object buffer and set attributes
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][PosBuffer]);
    glVertexAttribPointer(textureVPos, posCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(textureVPos);

    // Bind texture object buffer and set attributes
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][TexBuffer]);
    glVertexAttribPointer(textureVTex, texCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(textureVTex);

    // Draw object
    glDrawArrays(GL_TRIANGLES, 0, numVertices[obj]);
}



//UI related draw functions should at some point be migrated to kitUI
void draw_solid_hud_quad(vec4 color) {
    //setup dynamic buffer
    if (dynamicHUDColorBuffer == 0){
        glGenBuffers(1, &dynamicHUDColorBuffer);
        // allocate once to the size of the HUD quad
        vector<vec4> tmp(HUD_numVertices[HUDQuad], color);
        glBindBuffer(GL_ARRAY_BUFFER, dynamicHUDColorBuffer);
        glBufferData(GL_ARRAY_BUFFER, tmp.size() * sizeof(vec4), tmp.data(), GL_DYNAMIC_DRAW);
    }

    vector<vec4> cols(HUD_numVertices[HUDQuad], color);
    glBindBuffer(GL_ARRAY_BUFFER, dynamicHUDColorBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, cols.size() * sizeof(vec4), cols.data());

    //draw color object
    glUseProgram(colorProgram);
    glUniformMatrix4fv(colorProjMatLoc, 1, GL_FALSE, projMatrix);
    glUniformMatrix4fv(colorCamMatLoc, 1, GL_FALSE, cameraMatrix);
    glUniformMatrix4fv(colorModelMatLoc, 1, GL_FALSE, modelMatrix);

    glBindVertexArray(HUD_VAOs[HUDQuad]);
    glBindBuffer(GL_ARRAY_BUFFER, HUD_ObjBuffers[HUDQuad][PosBuffer]);
    glVertexAttribPointer(colorVPos, posCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(colorVPos);

    glBindBuffer(GL_ARRAY_BUFFER, dynamicHUDColorBuffer);
    glVertexAttribPointer(colorVCol, colCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(colorVCol);

    glDrawArrays(GL_TRIANGLES, 0, HUD_numVertices[HUDQuad]);
}


void draw_text_quad() {
    glUseProgram(hudProgram);

    glUniformMatrix4fv(hudProjMatLoc, 1, GL_FALSE, projMatrix);
    glUniformMatrix4fv(hudCameraMatLoc, 1, GL_FALSE, cameraMatrix);
    glUniformMatrix4fv(hudModelMatLoc, 1, GL_FALSE, modelMatrix);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fontTextureID);

    glBindVertexArray(HUD_VAOs[HUDTextQuad]);

    glBindBuffer(GL_ARRAY_BUFFER, HUD_ObjBuffers[HUDTextQuad][PosBuffer]);
    glVertexAttribPointer(hudVPos, posCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(hudVPos);

    glBindBuffer(GL_ARRAY_BUFFER, HUD_ObjBuffers[HUDTextQuad][TexBuffer]);
    glVertexAttribPointer(hudVTex, texCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(hudVTex);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}



#endif // DRAW_OBJECTS_H