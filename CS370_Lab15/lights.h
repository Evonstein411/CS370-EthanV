enum LightNames {WhiteSpotLight, NumLights};
GLuint LightBuffers[NumLightBuffers];
vector<LightProperties> Lights;
GLint lightOn[NumLights] = {0};

// Spot white light
LightProperties whiteSpotLight = {
		SPOT, //type
		{0.0f, 0.0f, 0.0f}, //pad
		vec4(0.1f, 0.1f, 0.1f, 1.0f), //ambient
		vec4(1.0f, 1.0f, 1.0f, 1.0f), //diffuse
		vec4(1.0f, 1.0f, 1.0f, 1.0f), //specular
		vec4(5.0f, 5.0f, 5.0f, 1.0f),  //position
		vec4(-1.0f, -1.0f, -1.0f, 0.0f), //direction
		30.0f,   //cutoff
		20.0f,  //exponent
		{0.0f, 0.0f}  //pad2
};

void build_lights( ) {
	// Allocate Lights vector
	Lights.resize(NumLights);
	// Add lights to Lights vector
	Lights[WhiteSpotLight] = whiteSpotLight;
	lightOn[WhiteSpotLight] = 1;

	// Create uniform buffer for lights
	glGenBuffers(NumLightBuffers, LightBuffers);
	glBindBuffer(GL_UNIFORM_BUFFER, LightBuffers[LightBuffer]);
	glBufferData(GL_UNIFORM_BUFFER, Lights.size()*sizeof(LightProperties), Lights.data(), GL_STATIC_DRAW);
}
