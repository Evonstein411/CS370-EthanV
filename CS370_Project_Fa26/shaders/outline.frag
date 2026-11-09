#version 400 core
in vec2 texCoord;
out vec4 FragColor;
uniform sampler2D maskTex;
uniform vec2 texelSize;              // 1.0 / window size
uniform vec4 outlineColor;
uniform float thickness;             // usually 1.0–2.0

void main() {
    float center = texture(maskTex, texCoord).r;
    float edge = 0.0;

    // Sample cardinal neighbors (simple 1-pixel edge)
    vec2 off = texelSize * thickness;
    float n = texture(maskTex, texCoord + vec2( 0.0,  off.y)).r;
    float s = texture(maskTex, texCoord + vec2( 0.0, -off.y)).r;
    float e = texture(maskTex, texCoord + vec2( off.x,  0.0)).r;
    float w = texture(maskTex, texCoord + vec2(-off.x,  0.0)).r;

    // Edge if center and any neighbor differ
    if (center != n || center != s || center != e || center != w)
    edge = 1.0;

    FragColor = vec4(outlineColor.rgb, edge * outlineColor.a);
}