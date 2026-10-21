enum LightNames {WhitePointLight, SphereLight, CubeLight, OctLight, TorusLight, NumLights};
GLuint LightBuffers[NumLightBuffers];
vector<LightProperties> Lights;
GLint lightOn[NumLights] = {0};

// Create white point light
LightProperties whitePointLight = {
		POINT, //type
		{0.0f, 0.0f, 0.0f}, //pad
		vec4(0.0f, 0.0f, 0.0f, 1.0f), //ambient
		vec4(1.0f, 1.0f, 1.0f, 1.0f), //diffuse
		vec4(1.0f, 1.0f, 1.0f, 1.0f), //specular
		vec4(0.0f, 5.0f, 0.0f, 1.0f),  //position
		vec4(0.0f, 0.0f, 0.0f, 0.0f), //direction
		0.0f,   //cutoff
		0.0f,  //exponent
		{0.0f, 0.0f}  //pad2
};

// TODO: Add lights
LightProperties sphereLight = {
	SPOT, //type
	{0.0f, 0.0f, 0.0f}, // pad
	vec4(0.0f, 0.0f, 0.0f, 1.0f), // ambient
	vec4(0.0f, 0.5f, 0.0f, 1.0f), // diffuse
	vec4(0.0f, 0.5f, 0.0f, 1.0f), // specular
	vec4(0.0f, 5.0f, -0.5f, 1.0f), // position
	vec4(0.0f, -1.0f, 0.0f, 0.0f), // direction
	15.0f,   // cutoff
	0.1f,    // exponent
	{0.0f, 0.0f} // pad2
};

LightProperties cubeLight = {
	SPOT, //type
	{0.0f, 0.0f, 0.0f}, // pad
	vec4(0.0f, 0.0f, 0.0f, 1.0f), // ambient
	vec4(0.5f, 0.0f, 0.0f, 1.0f), // diffuse
	vec4(0.5f, 0.0f, 0.0f, 1.0f), // specular
	vec4(-6.0f, 5.0f, -0.5f, 1.0f), // position
	vec4(0.0f, -1.0f, 0.0f, 0.0f), // direction
	15.0f,   // cutoff
	0.1f,    // exponent
	{0.0f, 0.0f} // pad2
};

LightProperties octLight = {
	SPOT, //type
	{0.0f, 0.0f, 0.0f}, // pad
	vec4(0.0f, 0.0f, 0.0f, 1.0f), // ambient
	vec4(0.0f, 0.0f, 1.0f, 1.0f), // diffuse
	vec4(0.0f, 0.0f, 1.0f, 1.0f), // specular
	vec4(6.0f, 5.0f, -0.5f, 1.0f), // position
	vec4(0.0f, -1.0f, 0.0f, 0.0f), // direction
	15.0f,   // cutoff
	0.1f,    // exponent
	{0.0f, 0.0f} // pad2
};

LightProperties torusLight = {
	SPOT, //type
	{0.0f, 0.0f, 0.0f}, // pad
	vec4(0.0f, 0.0f, 0.0f, 1.0f), // ambient
	vec4(0.25, 0.0f, 1.0f, 1.0f), // diffuse
	vec4(0.25, 0.0f, 1.0f, 1.0f), // specular
	vec4(0.0f, 5.0f, 5.5f, 1.0f), // position
	vec4(0.0f, -1.0f, 0.0f, 0.0f), // direction
	15.0f,   // cutoff
	0.1f,    // exponent
	{0.0f, 0.0f} // pad2
};

void build_lights( ) {
	// Allocate Lights vector
	Lights.resize(NumLights);
    // TODO: Add lights to Lights vector
    Lights[WhitePointLight] = whitePointLight;
    lightOn[WhitePointLight] = 1;

	Lights[SphereLight] = sphereLight;
	lightOn[SphereLight] = 1;

	Lights[CubeLight] = cubeLight;
	lightOn[CubeLight] = 1;

	Lights[OctLight] = octLight;
	lightOn[OctLight] = 1;

	Lights[TorusLight] =torusLight;
	lightOn[TorusLight] = 1;

	// Create uniform buffer for lights
	glGenBuffers(NumLightBuffers, LightBuffers);
	glBindBuffer(GL_UNIFORM_BUFFER, LightBuffers[LightBuffer]);
	glBufferData(GL_UNIFORM_BUFFER, Lights.size()*sizeof(LightProperties), Lights.data(), GL_STATIC_DRAW);
}
