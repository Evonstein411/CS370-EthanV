#version 400 core
uniform sampler2D tex;


in vec2 texCoord;
out vec4 fragColor;

void main()
{
    // Sample texture map
    fragColor = texture(tex, texCoord);
}
