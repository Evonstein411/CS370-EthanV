enum LightNames {WhiteDirLight, GreenPointLight, RedSpotLight, NumLights};
GLuint LightBuffers[NumLightBuffers];
vector<LightProperties> Lights;
GLint lightOn[NumLights] = {0};

// White directional light
LightProperties whiteDirLight = {
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

// Green point light
LightProperties greenPointLight = {
		POINT, //type
		{0.0f, 0.0f, 0.0f}, //pad
		vec4(0.0f, 0.0f, 0.0f, 1.0f), //ambient
		vec4(0.0f, 1.0f, 0.0f, 1.0f), //diffuse
		vec4(0.0f, 1.0f, 0.0f, 1.0f), //specular
		vec4(3.0f, 3.0f, 3.0f, 1.0f),  //position
		vec4(0.0f, 0.0f, 0.0f, 0.0f), //direction
		0.0f,   //cutoff
		0.0f,  //exponent
		{0.0f, 0.0f}  //pad2
};

//Red spot light
LightProperties redSpotLight = {
		SPOT, //type
		{0.0f, 0.0f, 0.0f}, //pad
		vec4(0.0f, 0.0f, 0.0f, 1.0f), //ambient
		vec4(0.0f, 1.0f, 0.0f, 1.0f), //diffuse
		vec4(1.0f, 1.0f, 1.0f, 1.0f), //specular
		vec4(0.0f, 6.0f, 0.0f, 1.0f),  //position
		vec4(0.0f, -1.0f, 0.0f, 0.0f), //direction
		30.0f,   //cutoff
		30.0f,  //exponent
		{0.0f, 0.0f}  //pad2
};

void build_lights( ) {
    // Allocate Lights vector
    Lights.resize(NumLights);
    // Add lights to Lights vector
    Lights[WhiteDirLight] = whiteDirLight;
    lightOn[WhiteDirLight] = 1;
    Lights[GreenPointLight] = greenPointLight;
    lightOn[GreenPointLight] = 1;
    Lights[RedSpotLight] = redSpotLight;
	lightOn[RedSpotLight] = 1;

    // Create uniform buffer for lights
    glGenBuffers(NumLightBuffers, LightBuffers);
    glBindBuffer(GL_UNIFORM_BUFFER, LightBuffers[LightBuffer]);
    glBufferData(GL_UNIFORM_BUFFER, Lights.size()*sizeof(LightProperties), Lights.data(), GL_STATIC_DRAW);
}
