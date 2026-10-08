#include "shadervars.h"

void draw_color_object(GLuint obj, GLuint color) {
    // Select shader program
    glUseProgram(color_program);

    // Pass projection matrix to shader
    glUniformMatrix4fv(color_proj_mat_loc, 1, GL_FALSE, proj_matrix);

    // Pass camera matrix to shader
    glUniformMatrix4fv(color_cam_mat_loc, 1, GL_FALSE, camera_matrix);

    // Pass model matrix to shader
    glUniformMatrix4fv(color_model_mat_loc, 1, GL_FALSE, model_matrix);

    // Bind vertex array
    glBindVertexArray(VAOs[obj]);

    // Bind position object buffer and set attributes
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][PosBuffer]);
    glVertexAttribPointer(color_vPos, posCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(color_vPos);

    // Bind color buffer and set attributes
    glBindBuffer(GL_ARRAY_BUFFER, ColorBuffers[color]);
    glVertexAttribPointer(color_vCol, colCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(color_vCol);

    // Draw geometry
    glDrawArrays(GL_TRIANGLES, 0, numVertices[obj]);
}

void draw_axes(GLuint obj, GLuint c_buff){
    model_matrix = mat4().identity();

    // Select default shader program
    glUseProgram(color_program);

    // Pass projection matrix to default shader
    glUniformMatrix4fv(color_proj_mat_loc, 1, GL_FALSE, proj_matrix);

    // Pass camera matrix to default shader
    glUniformMatrix4fv(color_cam_mat_loc, 1, GL_FALSE, camera_matrix);

    // Pass model matrix to default shader
    glUniformMatrix4fv(color_model_mat_loc, 1, GL_FALSE, model_matrix);

    // Bind vertex array
    glBindVertexArray(VAOs[obj]);

    // Bind position object buffer and set attributes for default shader
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][PosBuffer]);
    glVertexAttribPointer(color_vPos, posCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(color_vPos);

    // Bind color buffer and set attributes for default shader
    glBindBuffer(GL_ARRAY_BUFFER, ColorBuffers[c_buff]);
    glVertexAttribPointer(color_vCol, colCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(color_vCol);

    // Draw object
    glDrawArrays(GL_LINES, 0, 6);
}