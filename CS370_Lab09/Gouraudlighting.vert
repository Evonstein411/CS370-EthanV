#version 400 core

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec3 vNormal;

uniform mat4 proj_matrix;
uniform mat4 camera_matrix;
uniform mat4 model_matrix;
uniform mat4 normal_matrix;

// Light structure
struct LightProperties {
    int type;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 position;
    vec4 direction;
    float spotCutoff;
    float spotExponent;
};

// Material structure
struct MaterialProperties {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

const int MaxLights = 8;
layout (std140) uniform LightBuffer {
    LightProperties Lights[MaxLights];
};

const int MaxMaterials = 8;
layout (std140) uniform MaterialBuffer {
    MaterialProperties Materials[MaxMaterials];
};

// Selected material
uniform int Material;

// Number of lights
uniform int NumLights;
uniform int LightOn[MaxLights];

uniform vec3 EyePosition;

out vec3 oColor;

void main( )
{

    // Compute transformed vertex position in view space
    gl_Position = proj_matrix*(camera_matrix*(model_matrix*vPosition));

    vec3 rgb = vec3(0.0f);
    float diff = 0.0;
    float spec = 0.0;
	for (int i = 0; i < NumLights; i++) {
		// If light is not off
		if (LightOn[i] != 0) {
			// TODO: Compute l
			
		
			// TODO: Compute n (transformed by normal matrix)
			
		
			// TODO: Compute v (camera location - transformed vertex)
			
		
			// TODO: Compute h
			
		
			// TODO: Compute ambient term
			
		
			// TODO: Compute diffuse term (Lambert's law)
			
		
			// Only add specular if there is diffuse
			if (diff != 0.0) {
				// TODO: Compute specular term
				
				
			}
		}
	}
    oColor = rgb;
}
