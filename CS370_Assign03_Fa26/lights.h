enum LightNames {WhitePointLight, SphereLight, CubeLight, OctLight, TorusLight, NumLights};
GLuint LightBuffers[NumLightBuffers];
vector<LightProperties> lights;
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
	lights.resize(NumLights);
    // TODO: Add lights to Lights vector
    lights[WhitePointLight] = whitePointLight;
    lightOn[WhitePointLight] = 1;

	lights[SphereLight] = sphereLight;
	lightOn[SphereLight] = 1;

	lights[CubeLight] = cubeLight;
	lightOn[CubeLight] = 1;

	lights[OctLight] = octLight;
	lightOn[OctLight] = 1;

	lights[TorusLight] =torusLight;
	lightOn[TorusLight] = 1;

	// Create uniform buffer for lights
	glGenBuffers(NumLightBuffers, LightBuffers);
	glBindBuffer(GL_UNIFORM_BUFFER, LightBuffers[LightBuffer]);
	glBufferData(GL_UNIFORM_BUFFER, lights.size()*sizeof(LightProperties), lights.data(), GL_STATIC_DRAW);
}
