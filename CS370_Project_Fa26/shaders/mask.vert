#version 400 core
layout(location = 0) in vec4 vPosition;
uniform mat4 proj_matrix;
uniform mat4 camera_matrix;
uniform mat4 model_matrix;
void main() {
    gl_Position = proj_matrix * camera_matrix * model_matrix * vPosition;
}