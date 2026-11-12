enum LightNames {WhitePointLight, NumLights};
GLuint LightBuffers[NumLightBuffers];
vector<LightProperties> lights;
GLint lightOn[NumLights] = {0};

// White point light
LightProperties whitePointLight = {
		POINT, //type
		{0.0f, 0.0f, 0.0f}, //pad
		vec4(0.0f, 0.0f, 0.0f, 1.0f), //ambient
		vec4(1.0f, 1.0f, 1.0f, 1.0f), //diffuse
		vec4(1.0f, 1.0f, 1.0f, 1.0f), //specular
		vec4(3.0f, 1.0f, 3.0f, 1.0f),  //position
		vec4(0.0f, 0.0f, 0.0f, 0.0f), //direction
		0.0f,   //cutoff
		0.0f,  //exponent
		{0.0f, 0.0f}  //pad2
};

void build_lights( ) {
	// Allocate Lights vector
	lights.resize(NumLights);
    // Add lights to Lights vector
    lights[WhitePointLight] = whitePointLight;
    lightOn[WhitePointLight] = 1;

	// Create uniform buffer for lights
	glGenBuffers(NumLightBuffers, LightBuffers);
	glBindBuffer(GL_UNIFORM_BUFFER, LightBuffers[LightBuffer]);
	glBufferData(GL_UNIFORM_BUFFER, lights.size()*sizeof(LightProperties), lights.data(), GL_STATIC_DRAW);
}
