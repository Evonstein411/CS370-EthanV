#version 400 core
uniform sampler2D tex;

out vec4 fragColor;

in vec2 texCoord;

void main()
{
    // Sample texture map
    float coverage = texture(tex, texCoord).r;
    fragColor = vec4(1.0, 1.0, 1.0, coverage);

}
