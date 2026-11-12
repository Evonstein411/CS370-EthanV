// Debug mirror renderer
unsigned int quadVAO = 0;
unsigned int quadVBO;

void debugTex(GLuint obj, GLuint tex)
{
    // reset viewport
    glViewport(0, 0, ww, hh);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	projMatrix = mat4().identity();
    cameraMatrix = mat4().identity();
    modelMatrix = rotate(180.0f, vec3(0.0f, 0.0f, 1.0f))*rotate(90.0f, vec3(1.0f, 0.0f, 0.0f));
    draw_tex_object(obj, tex);
}
