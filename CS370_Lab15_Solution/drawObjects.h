#include "shadervars.h"

void draw_mat_shadow_object(GLuint obj, GLuint material){
    // Generic shader variables references
    GLuint vPos;
    GLuint vNorm;
    GLuint model_mat_loc;

    // Reference appropriate shader variables
    if (shadow) {
        // Use shadow shader
        glUseProgram(shadow_program);
        // Pass shadow projection and camera matrices to shader
        glUniformMatrix4fv(shadow_proj_mat_loc, 1, GL_FALSE, shadow_proj_matrix);
        glUniformMatrix4fv(shadow_camera_mat_loc, 1, GL_FALSE, shadow_camera_matrix);

        // Set object attributes to shadow shader
        vPos = shadow_vPos;
        model_mat_loc = shadow_model_mat_loc;
    } else {
        // Use lighting shader with shadows
        glUseProgram(phong_shadow_program);

        // Pass object projection and camera matrices to shader
        glUniformMatrix4fv(phong_shadow_proj_mat_loc, 1, GL_FALSE, proj_matrix);
        glUniformMatrix4fv(phong_shadow_camera_mat_loc, 1, GL_FALSE, camera_matrix);

        // Bind lights
        glUniformBlockBinding(phong_shadow_program, phong_shadow_lights_block_idx, 0);
        glBindBufferRange(GL_UNIFORM_BUFFER, 0, LightBuffers[LightBuffer], 0, Lights.size() * sizeof(LightProperties));

        // Bind materials
        glUniformBlockBinding(phong_shadow_program, phong_shadow_materials_block_idx, 1);
        glBindBufferRange(GL_UNIFORM_BUFFER, 1, MaterialBuffers[MaterialBuffer], 0,
                          Materials.size() * sizeof(MaterialProperties));

        // Set camera position
        glUniform3fv(phong_shadow_eye_loc, 1, eye);

        // Set num lights and lightOn
        glUniform1i(phong_shadow_num_lights_loc, Lights.size());
        glUniform1iv(phong_shadow_light_on_loc, NumLights, lightOn);

        // Pass normal matrix to shader
        glUniformMatrix4fv(phong_shadow_norm_mat_loc, 1, GL_FALSE, normal_matrix);

        // Pass material index to shader
        glUniform1i(phong_shadow_material_loc, material);

        // TODO: Pass shadow projection and camera matrices
        glUniformMatrix4fv(phong_shadow_shad_proj_mat_loc, 1, GL_FALSE, shadow_proj_matrix);
        glUniformMatrix4fv(phong_shadow_shad_cam_mat_loc, 1, GL_FALSE, shadow_camera_matrix);

        // TODO: Bind shadow texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ShadowTextureIDs[ShadowTex1]);

        // Set object attributes for phong shadow shader
        vPos = phong_shadow_vPos;
        vNorm = phong_shadow_vNorm;
        model_mat_loc = phong_shadow_model_mat_loc;
    }

    // Pass model matrix to shader
    glUniformMatrix4fv(model_mat_loc, 1, GL_FALSE, model_matrix);

    // Bind vertex array
    glBindVertexArray(VAOs[obj]);

    // Bind position object buffer and set attributes
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][PosBuffer]);
    glVertexAttribPointer(vPos, posCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(vPos);

    if (!shadow) {
        // Bind object normal buffer if using phong shadow shader
        glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][NormBuffer]);
        glVertexAttribPointer(vNorm, normCoords, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(vNorm);
    }

    // Draw object
    glDrawArrays(GL_TRIANGLES, 0, numVertices[obj]);
}