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

void draw_axes(GLuint a_obj, GLuint a_buff){
    // Select default shader program
    glUseProgram(colorProgram);

    // Pass projection matrix to default shader
    glUniformMatrix4fv(colorProjMatLoc, 1, GL_FALSE, projMatrix);

    // Pass camera matrix to default shader
    glUniformMatrix4fv(colorCamMatLoc, 1, GL_FALSE, cameraMatrix);

    // Pass model matrix to default shader
    glUniformMatrix4fv(colorModelMatLoc, 1, GL_FALSE, modelMatrix);

    // Bind vertex array
    glBindVertexArray(VAOs[a_obj]);

    // Bind position object buffer and set attributes for default shader
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[a_obj][PosBuffer]);
    glVertexAttribPointer(colorVPos, posCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(colorVPos);

    // Bind color buffer and set attributes for default shader
    glBindBuffer(GL_ARRAY_BUFFER, ColorBuffers[a_buff]);
    glVertexAttribPointer(colorVCol, colCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(colorVCol);

    // Draw object
    glDrawArrays(GL_LINES, 0, numVertices[a_obj]);
}
