#version 400 core
layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec2 vTexCoord;
out vec2 texCoord;
void main() {
    gl_Position = vPosition;          // already in NDC for the HUD quad
    texCoord = vTexCoord;
}