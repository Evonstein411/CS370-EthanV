#include "shadervars.h"

void draw_color_object(GLuint obj, GLuint color) {
    // TODO: Select color shader program


    // TODO: Pass projection matrix to color shader


    // TODO: Pass camera matrix to color shader

    // TODO: Pass model matrix to color shader


    // Bind vertex array
    glBindVertexArray(VAOs[obj]);

    // TODO: Bind position object buffer and set attributes for color shader


    // TODO: Bind color buffer and set attributes for color shader


    // Draw object
    glDrawArrays(GL_TRIANGLES, 0, numVertices[obj]);
}

void draw_dim_color_object(GLuint obj, GLuint color, GLfloat dim) {
    // TODO: Select dimmed shader program


    // TODO: Pass projection matrix to dim shader


    // TODO: Pass camera matrix to dim shader


    // TODO: Pass model matrix to dim shader


    // Bind vertex array
    glBindVertexArray(VAOs[obj]);

    // TODO: Bind position object buffer and set attributes for dim shader


    // TODO: Bind color buffer and set attributes for color shader


    // TODO: Pass dim variable (float) to dim shader


    // Draw object
    glDrawArrays(GL_TRIANGLES, 0, numVertices[obj]);
}
