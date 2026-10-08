enum LightNames {WhitePointLight, FillLight, NumLights};
GLuint LightBuffers[NumLightBuffers];
vector<LightProperties> Lights;
GLint lightOn[NumLights] = {0, 0};

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

// White directional light
LightProperties fillLight = {
	DIRECTIONAL,
	{0.0f, 0.0f, 0.0f},
	vec4(0.15f, 0.15f, 0.15f, 1.0f),   // ambient, so it never goes fully black
	vec4(0.25f, 0.25f, 0.28f, 1.0f),   // diffuse, weaker than the point light
	vec4(0.10f, 0.10f, 0.10f, 1.0f),   // specular
	vec4(0.0f, 0.0f, 0.0f, 0.0f),      // position unused
	vec4(-0.3f, -1.0f, -0.4f, 0.0f),   // rays travel down and toward -z
	0.0f,
	0.0f,
	{0.0f, 0.0f}
};


void build_lights( ) {
	// Allocate Lights vector
	Lights.resize(NumLights);
	// Add lights to Lights vector
	Lights[WhitePointLight] = whitePointLight;
	lightOn[WhitePointLight] = 1;

	Lights[FillLight] = fillLight;
	lightOn[FillLight] = 1;

	// Create uniform buffer for lights
	glGenBuffers(NumLightBuffers, LightBuffers);
	glBindBuffer(GL_UNIFORM_BUFFER, LightBuffers[LightBuffer]);
	glBufferData(GL_UNIFORM_BUFFER, Lights.size()*sizeof(LightProperties), Lights.data(), GL_STATIC_DRAW);
}
