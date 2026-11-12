#include "shadervars.h"

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

void draw_color_fan_object(GLuint obj, GLuint color) {
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
    glDrawArrays(GL_TRIANGLE_FAN, 0, numVertices[obj]);
}
