#include "shadervars.h"

void draw_fog_color_object(GLuint obj, GLuint color) {
    // Select shader program
    glUseProgram(fog_program);
    // Pass projection matrix to shader
    glUniformMatrix4fv(fog_proj_mat_loc, 1, GL_FALSE, proj_matrix);

    // Pass camera matrix to shader
    glUniformMatrix4fv(fog_cam_mat_loc, 1, GL_FALSE, camera_matrix);

    // Pass model matrix to shader
    glUniformMatrix4fv(fog_model_mat_loc, 1, GL_FALSE, model_matrix);

    // Pass fog values to shader
    glUniform4fv(fog_fog_col_loc, 1, fogColor);
    glUniform1f(fog_fog_start_loc, fogStart);
    glUniform1f(fog_fog_end_loc, fogEnd);

    // Bind vertex array
    glBindVertexArray(VAOs[obj]);

    // Bind position object buffer and set attributes for fog shader
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][PosBuffer]);
    glVertexAttribPointer(fog_vPos, posCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(fog_vPos);

    // Bind color buffer and set attributes for fog shader
    glBindBuffer(GL_ARRAY_BUFFER, ColorBuffers[color]);
    glVertexAttribPointer(fog_vCol, colCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(fog_vCol);

    // Draw object
    glDrawArrays(GL_TRIANGLES, 0, numVertices[obj]);
}
