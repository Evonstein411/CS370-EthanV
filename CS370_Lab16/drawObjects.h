#include "shadervars.h"

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
    glBindBufferRange(GL_UNIFORM_BUFFER, 1, MaterialBuffers[MaterialBuffer], 0, materials.size()*sizeof(MaterialProperties));

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

void draw_tex_object(GLuint obj, GLuint texture){
    // Select shader program
    glUseProgram(textureProgram);

    // Pass projection matrix to shader
    glUniformMatrix4fv(textureProjMatLoc, 1, GL_FALSE, projMatrix);

    // Pass camera matrix to shader
    glUniformMatrix4fv(textureCameraMatLoc, 1, GL_FALSE, cameraMatrix);

    // Pass model matrix to shader
    glUniformMatrix4fv(textureModelMatLoc, 1, GL_FALSE, modelMatrix);

    // Bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TextureIDs[texture]);

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

void draw_frame(GLuint obj){
    // Draw frame using lines at mirror location
    glUseProgram(lightingProgram);
    // Pass projection and camera matrices to shader
    glUniformMatrix4fv(lightingProjMatLoc, 1, GL_FALSE, projMatrix);
    glUniformMatrix4fv(lightingCameraMatLoc, 1, GL_FALSE, cameraMatrix);

    // Bind lights
    glUniformBlockBinding(lightingProgram, lightingLightsBlockIdx, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, LightBuffers[LightBuffer], 0, lights.size()*sizeof(LightProperties));
    // Bind materials
    glUniformBlockBinding(lightingProgram, lightingMaterialsBlockIdx, 1);
    glBindBufferRange(GL_UNIFORM_BUFFER, 1, MaterialBuffers[MaterialBuffer], 0, materials.size()*sizeof(MaterialProperties));
    // Set camera position
    glUniform3fv(lightingEyeLoc, 1, eye);
    // Set num lights and lightOn
    glUniform1i(lightingNumLightsLoc, lights.size());
    glUniform1iv(lightingLightOnLoc, NumLights, lightOn);

    // Set frame transformation matrix
    mat4 trans_matrix = translate(mirror_eye);
    mat4 rot_matrix = rotate(-90.0f, vec3(1.0f, 0.0f, 0.0f));
    mat4 scale_matrix = scale(2.0f, 1.0f, 2.0f);
    modelMatrix = trans_matrix * rot_matrix * scale_matrix;
    // Compute normal matrix from model matrix
    normalMatrix = modelMatrix.inverse().transpose();
    // Pass model matrix and normal matrix to shader
    glUniformMatrix4fv(lightingModelMatLoc, 1, GL_FALSE, modelMatrix);
    glUniformMatrix4fv(lightingNormMatLoc, 1, GL_FALSE, normalMatrix);
    glUniform1i(lightingMaterialLoc, RedPlastic);

    // Draw object using line loop
    glBindVertexArray(VAOs[obj]);
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][PosBuffer]);
    glVertexAttribPointer(lightingVPos, posCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(lightingVPos);
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][NormBuffer]);
    glVertexAttribPointer(lightingVNorm, normCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(lightingVNorm);
    // Draw as line loop
    glDrawArrays(GL_LINE_LOOP, 0, numVertices[obj]);
}
