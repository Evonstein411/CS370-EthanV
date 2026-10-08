enum LightNames {WhiteLight, NumLights};
GLuint LightBuffers[NumLightBuffers];
vector<LightProperties> Lights;
GLint lightOn[NumLights] = {0};

// TODO: Create white directional light
LightProperties whiteLight = {
		DIRECTIONAL, //type
		{0.0f, 0.0f, 0.0f}, //pad
		vec4(0.0f, 0.0f, 0.0f, 1.0f), //ambient
		vec4(1.0f, 1.0f, 1.0f, 1.0f), //diffuse
		vec4(1.0f, 1.0f, 1.0f, 1.0f), //specular
		vec4(0.0f, 0.0f, 0.0f, 1.0f),  //position
		vec4(-1.0f, -1.0f, -1.0f, 0.0f), //direction
		0.0f,   //cutoff
		0.0f,  //exponent
		{0.0f, 0.0f}  //pad2
};


void build_lights( ) {
	// Allocate Lights vector
	Lights.resize(NumLights);
    // TODO: Add lights to Lights vector
    Lights[WhiteLight] = whiteLight;
    lightOn[WhiteLight] = 1;
    
	// Create uniform buffer for lights
	glGenBuffers(NumLightBuffers, LightBuffers);
	glBindBuffer(GL_UNIFORM_BUFFER, LightBuffers[LightBuffer]);
	glBufferData(GL_UNIFORM_BUFFER, Lights.size()*sizeof(LightProperties), Lights.data(), GL_STATIC_DRAW);
}
