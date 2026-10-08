#include "shadervars.h"

void draw_basic_object(GLuint obj) {
    // TODO: Select shader program
    glUseProgram(basic_program);

    // TODO: Bind vertex array
    glBindVertexArray(VAOs[obj]);

    // TODO: Bind position object buffer and set attributes
    glBindBuffer(GL_ARRAY_BUFFER, ObjBuffers[obj][PosBuffer]);
    glVertexAttribPointer(basic_vPos, posCoords, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(basic_vPos);

    // TODO: Draw geometry
    glDrawArrays(GL_TRIANGLES, 0, numVertices[obj]);

}
