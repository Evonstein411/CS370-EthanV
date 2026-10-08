#include "shadervars.h"

void draw_color_object(GLuint obj, GLuint color) {
    // TODO: Select color shader program
    glUseProgram(color_program);

    // TODO: Pass projection matrix to color shader
    glUniformMatrix4fv(color_proj_mat_loc, 1, GL_FALSE, proj_matrix);

    // TODO: Pass camera matrix to color shader
    glUniformMatrix4fv(color_cam_mat_loc, 1, GL_FALSE, camera_matrix);

    // TODO: Pass model matrix to color shader
    glUniformMatrix4fv(color_model_mat_loc, 1, GL_FALSE, model_matrix);

    // Bind vertex array
    glBindVertexArray(VAOs[obj]);

    // TODO: Bind position object buffer and set attributes for color shader
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][PosBuffer]);
    glVertexAttribPointer(color_vPos, posCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(color_vPos);

    // TODO: Bind color buffer and set attributes for color shader
    glBindBuffer(GL_ARRAY_BUFFER, ColorBuffers[color]);
    glVertexAttribPointer(color_vCol, colCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(color_vCol);

    // Draw object
    glDrawArrays(GL_TRIANGLES, 0, numVertices[obj]);
}

void draw_dim_color_object(GLuint obj, GLuint color, GLfloat dim) {
    // TODO: Select dimmed shader program
    glUseProgram(dim_program);

    // TODO: Pass projection matrix to dim shader
    glUniformMatrix4fv(dim_proj_mat_loc, 1, GL_FALSE, proj_matrix);

    // TODO: Pass camera matrix to dim shader
    glUniformMatrix4fv(dim_cam_mat_loc, 1, GL_FALSE, camera_matrix);

    // TODO: Pass model matrix to dim shader
    glUniformMatrix4fv(dim_model_mat_loc, 1, GL_FALSE, model_matrix);

    // Bind vertex array
    glBindVertexArray(VAOs[obj]);

    // TODO: Bind position object buffer and set attributes for dim shader
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][PosBuffer]);
    glVertexAttribPointer(dim_vPos, posCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(dim_vPos);

    // TODO: Bind color buffer and set attributes for color shader
    glBindBuffer(GL_ARRAY_BUFFER, ColorBuffers[color]);
    glVertexAttribPointer(dim_vCol, colCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(dim_vCol);

    // TODO: Pass dim variable (float) to dim shader
    glUniform1f(dim_factor_loc, dim);

    // Draw object
    glDrawArrays(GL_TRIANGLES, 0, numVertices[obj]);
}
