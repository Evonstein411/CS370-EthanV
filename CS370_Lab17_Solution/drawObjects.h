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

void draw_multi_tex_object(GLuint obj, GLuint baseID, GLuint blendID, GLfloat mix){
    // Select shader program
    glUseProgram(multi_tex_program);

    // Pass projection matrix to shader
    glUniformMatrix4fv(multi_tex_proj_mat_loc, 1, GL_FALSE, projMatrix);

    // Pass camera matrix to shader
    glUniformMatrix4fv(multi_tex_camera_mat_loc, 1, GL_FALSE, cameraMatrix);

    // Pass model matrix to shader
    glUniformMatrix4fv(multi_tex_model_mat_loc, 1, GL_FALSE, modelMatrix);

    // TODO: Pass mix factor to shader
    glUniform1f(multi_tex_mix_loc, mix);

    // TODO: Set base texture to texture unit 0 and make it active
    glUniform1i(multi_tex_base_loc, 0);
    glActiveTexture(GL_TEXTURE0);
    // TODO: Bind base texture (to unit 0)
    glBindTexture(GL_TEXTURE_2D, TextureIDs[baseID]);

    // TODO: Set second texture to texture unit 1 and make it active
    glUniform1i(multi_tex_blend_loc, 1);
    glActiveTexture(GL_TEXTURE1);
    // TODO: Bind second texture (to unit 1)
    glBindTexture(GL_TEXTURE_2D, TextureIDs[blendID]);

    // Bind vertex array
    glBindVertexArray(VAOs[obj]);

    // Bind position object buffer and set attributes
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][PosBuffer]);
    glVertexAttribPointer(multi_tex_vPos, posCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(multi_tex_vPos);

    // Bind texture object buffer and set attributes
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][TexBuffer]);
    glVertexAttribPointer(multi_tex_vTex, texCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(multi_tex_vTex);

    // Draw object
    glDrawArrays(GL_TRIANGLES, 0, numVertices[obj]);
}
