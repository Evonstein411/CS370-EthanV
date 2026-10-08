#include "shadervars.h"

void draw_mat_object(GLuint obj, GLuint material){
    // Select shader program
    glUseProgram(lighting_program);

    // Pass projection and camera matrices to shader
    glUniformMatrix4fv(lighting_proj_mat_loc, 1, GL_FALSE, proj_matrix);
    glUniformMatrix4fv(lighting_camera_mat_loc, 1, GL_FALSE, camera_matrix);

    // Bind lights
    glUniformBlockBinding(lighting_program, lighting_lights_block_idx, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, LightBuffers[LightBuffer], 0, Lights.size()*sizeof(LightProperties));

    // Bind materials
    glUniformBlockBinding(lighting_program, lighting_materials_block_idx, 1);
    glBindBufferRange(GL_UNIFORM_BUFFER, 1, MaterialBuffers[MaterialBuffer], 0, Materials.size()*sizeof(MaterialProperties));

    // Set camera position
    glUniform3fv(lighting_eye_loc, 1, eye);

    // Set num lights and lightOn
    glUniform1i(lighting_num_lights_loc, Lights.size());
    glUniform1iv(lighting_light_on_loc, NumLights, lightOn);

    // Pass model matrix and normal matrix to shader
    glUniformMatrix4fv(lighting_model_mat_loc, 1, GL_FALSE, model_matrix);
    glUniformMatrix4fv(lighting_norm_mat_loc, 1, GL_FALSE, normal_matrix);

    // Pass material index to shader
    glUniform1i(lighting_material_loc, material);

    // Bind vertex array
    glBindVertexArray(VAOs[obj]);

    // Bind position object buffer and set attributes
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][PosBuffer]);
    glVertexAttribPointer(lighting_vPos, posCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(lighting_vPos);

    // Bind normal object buffer and set attributes
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][NormBuffer]);
    glVertexAttribPointer(lighting_vNorm, normCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(lighting_vNorm);

    // Draw object
    glDrawArrays(GL_TRIANGLES, 0, numVertices[obj]);
}

void draw_multi_tex_object(GLuint obj, GLuint baseID, GLuint blendID, GLfloat mix){
    // Select shader program
    glUseProgram(multi_tex_program);

    // Pass projection matrix to shader
    glUniformMatrix4fv(multi_tex_proj_mat_loc, 1, GL_FALSE, proj_matrix);

    // Pass camera matrix to shader
    glUniformMatrix4fv(multi_tex_camera_mat_loc, 1, GL_FALSE, camera_matrix);

    // Pass model matrix to shader
    glUniformMatrix4fv(multi_tex_model_mat_loc, 1, GL_FALSE, model_matrix);

    // Pass mix factor to shader
    glUniform1f(multi_tex_mix_loc, mix);

    // Set base texture to texture unit 0 and make it active
    glUniform1i(multi_tex_base_loc, 0);
    glActiveTexture(GL_TEXTURE0);
    // Bind base texture (to unit 0)
    glBindTexture(GL_TEXTURE_2D, TextureIDs[baseID]);

    // Set second texture to texture unit 1 and make it active
    glUniform1i(multi_tex_blend_loc, 1);
    glActiveTexture(GL_TEXTURE1);
    // Bind second texture (to unit 1)
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

void draw_bump_object(GLuint obj, GLuint baseID, GLuint normalID){
    // Select shader program
    glUseProgram(bump_program);

    // Pass projection and camera matrices to shader
    glUniformMatrix4fv(bump_proj_mat_loc, 1, GL_FALSE, proj_matrix);
    glUniformMatrix4fv(bump_camera_mat_loc, 1, GL_FALSE, camera_matrix);

    // Bind lights
    glUniformBlockBinding(bump_program, bump_lights_block_idx, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, LightBuffers[LightBuffer], 0, Lights.size() * sizeof(LightProperties));

    // Set camera position
    glUniform3fv(bump_eye_loc, 1, eye);

    // Set num lights and lightOn
    glUniform1i(bump_num_lights_loc, Lights.size());
    glUniform1iv(bump_light_on_loc, NumLights, lightOn);

    // Pass model matrix and normal matrix to shader
    glUniformMatrix4fv(bump_model_mat_loc, 1, GL_FALSE, model_matrix);
    glUniformMatrix4fv(bump_norm_mat_loc, 1, GL_FALSE, normal_matrix);

    // Set base texture to texture unit 0 and make it active
    glUniform1i(bump_base_loc, 0);
    glActiveTexture(GL_TEXTURE0);
    // Bind base texture (to unit 0)
    glBindTexture(GL_TEXTURE_2D, TextureIDs[baseID]);

    // Set normal map texture to texture unit 1 and make it active
    glUniform1i(bump_norm_loc, 1);
    glActiveTexture(GL_TEXTURE1);
    // Bind normal map texture (to unit 1)
    glBindTexture(GL_TEXTURE_2D, TextureIDs[normalID]);

    // Bind vertex array
    glBindVertexArray(VAOs[obj]);

    // Bind position object buffer and set attributes
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][PosBuffer]);
    glVertexAttribPointer(bump_vPos, posCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(bump_vPos);

    // Bind normal object buffer and set attributes
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][NormBuffer]);
    glVertexAttribPointer(bump_vNorm, normCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(bump_vNorm);

    // Bind texture object buffer and set attributes
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][TexBuffer]);
    glVertexAttribPointer(bump_vTex, texCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(bump_vTex);

    // Bind tangent object buffer and set attributes
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][TangBuffer]);
    glVertexAttribPointer(bump_vTang, tangCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(bump_vTang);

    // Bind bitangent object buffer and set attributes
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][BiTangBuffer]);
    glVertexAttribPointer(bump_vBiTang, bitangCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(bump_vBiTang);

    // Draw object
    glDrawArrays(GL_TRIANGLES, 0, numVertices[obj]);
}
