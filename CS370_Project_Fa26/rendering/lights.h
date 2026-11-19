#ifndef LIGHTS_H
#define LIGHTS_H


#include "../common/lighting.h"
#include <vector>

enum LightBuffer_IDs {LightBuffer, NumLightBuffers};
enum LightNames {WhitePointLight, FillLight, NumLights};

inline GLuint LightBuffers[NumLightBuffers];
inline std::vector<LightProperties> lights;
inline GLint lightOn[NumLights] = {0, 0};

// White point light
inline LightProperties whitePointLight = {
		POINT, //type
		{0.0f, 0.0f, 0.0f}, //pad
		vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f), //ambient
		vmath::vec4(1.0f, 1.0f, 1.0f, 1.0f), //diffuse
		vmath::vec4(1.0f, 1.0f, 1.0f, 1.0f), //specular
		vmath::vec4(3.0f, 1.0f, 3.0f, 1.0f),  //position
		vmath::vec4(0.0f, 0.0f, 0.0f, 0.0f), //direction
		0.0f,   //cutoff
		0.0f,  //exponent
		{0.0f, 0.0f}  //pad2
};

// White directional light
inline LightProperties fillLight = {
	DIRECTIONAL,
	{0.0f, 0.0f, 0.0f},
	vmath::vec4(0.15f, 0.15f, 0.15f, 1.0f),   // ambient, so it never goes fully black
	vmath::vec4(0.25f, 0.25f, 0.28f, 1.0f),   // diffuse, weaker than the point light
	vmath::vec4(0.10f, 0.10f, 0.10f, 1.0f),   // specular
	vmath::vec4(0.0f, 0.0f, 0.0f, 0.0f),      // position unused
	vmath::vec4(-0.3f, -1.0f, -0.4f, 0.0f),   // rays travel down and toward -z
	0.0f,
	0.0f,
	{0.0f, 0.0f}
};


inline void build_lights( ) {
	// Allocate Lights vector
	lights.resize(NumLights);
	// Add lights to Lights vector
	lights[WhitePointLight] = whitePointLight;
	lightOn[WhitePointLight] = 1;

	lights[FillLight] = fillLight;
	lightOn[FillLight] = 1;

	// Create uniform buffer for lights
	glGenBuffers(NumLightBuffers, LightBuffers);
	glBindBuffer(GL_UNIFORM_BUFFER, LightBuffers[LightBuffer]);
	glBufferData(GL_UNIFORM_BUFFER, lights.size()*sizeof(LightProperties), lights.data(), GL_STATIC_DRAW);
}

#endif //LIGHTS_H