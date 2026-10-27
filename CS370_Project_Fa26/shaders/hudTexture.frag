#version 400 core
uniform sampler2D tex;
uniform vec4 textColor;


in vec2 texCoord;
out vec4 fragColor;


void main()
{
    // Sample texture map
    float coverage = texture(tex, texCoord).r;
    fragColor = vec4(textColor.rgb, textColor.a * coverage);

}
