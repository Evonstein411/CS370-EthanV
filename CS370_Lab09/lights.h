enum LightNames {WhiteLight, NumLights};
GLuint LightBuffers[NumLightBuffers];
vector<LightProperties> lights;
GLint lightOn[NumLights] = {0};

// TODO: Create white directional light


void build_lights( ) {
	// Allocate Lights vector
	lights.resize(NumLights);
    // TODO: Add lights to Lights vector

    
	// Create uniform buffer for lights
	glGenBuffers(NumLightBuffers, LightBuffers);
	glBindBuffer(GL_UNIFORM_BUFFER, LightBuffers[LightBuffer]);
	glBufferData(GL_UNIFORM_BUFFER, lights.size()*sizeof(LightProperties), lights.data(), GL_STATIC_DRAW);
}
