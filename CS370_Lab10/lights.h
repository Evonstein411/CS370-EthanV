// TODO: Add GreenSpotLight constant
enum LightNames {WhitePointLight, NumLights};
GLuint LightBuffers[NumLightBuffers];
vector<LightProperties> Lights;
GLint lightOn[NumLights] = {0};

// TODO: Create white point light


// TODO: Create green spot light


void build_lights( ) {
	// Allocate Lights vector
	Lights.resize(NumLights);
    // TODO: Add lights to Lights vector



	// Create uniform buffer for lights
	glGenBuffers(NumLightBuffers, LightBuffers);
	glBindBuffer(GL_UNIFORM_BUFFER, LightBuffers[LightBuffer]);
	glBufferData(GL_UNIFORM_BUFFER, Lights.size()*sizeof(LightProperties), Lights.data(), GL_STATIC_DRAW);
}
