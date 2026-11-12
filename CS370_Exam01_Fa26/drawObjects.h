#include "shadervars.h"

// ---------------------
// Fa26 DO NOT MODIFY!!!
// ---------------------

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

void draw_target(GLuint t_obj, GLuint t_buff){
    // Select default shader program
    glUseProgram(colorProgram);

    // Pass projection matrix to default shader
    glUniformMatrix4fv(colorProjMatLoc, 1, GL_FALSE, projMatrix);

    // Pass camera matrix to default shader
    glUniformMatrix4fv(colorCamMatLoc, 1, GL_FALSE, cameraMatrix);

    // Pass model matrix to default shader
    glUniformMatrix4fv(colorModelMatLoc, 1, GL_FALSE, modelMatrix);

    // Bind vertex array
    glBindVertexArray(VAOs[t_obj]);

    // Bind position object buffer and set attributes for default shader
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[t_obj][PosBuffer]);
    glVertexAttribPointer(colorVPos, posCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(colorVPos);

    // Bind color buffer and set attributes for default shader
    glBindBuffer(GL_ARRAY_BUFFER, ColorBuffers[t_buff]);
    glVertexAttribPointer(colorVCol, colCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(colorVCol);

    // Draw object
    glDrawArrays(GL_LINES, 0, 4);
    if (mode > 2) {
        glDrawArrays(GL_LINE_LOOP, 4, 64);
    }

}

void draw_table(){
    mat4 scale_matrix = scale(TABLE_SIZE, 0.02f, TABLE_SIZE);
    mat4 trans_matrix = translate(0.0f, -0.2f, 0.0f);
    modelMatrix = scale_matrix;
    draw_color_object(Cube, TableColor);

    if (mode < 4) {
        modelMatrix = mat4().identity();
    } else {
        mat4 targ_trans = translate(TARGET_X, TARGET_Y, TARGET_Z);
        modelMatrix = targ_trans;
    }
    draw_target(Target, TargetColor);
}
