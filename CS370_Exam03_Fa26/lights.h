enum LightNames {WhiteDirLight, RedSpotLight, RedTreeLight1, GreenTreeLight1, RedTreeLight2, GreenTreeLight2, NumLights};
GLuint LightBuffers[NumLightBuffers];
vector<LightProperties> Lights;
GLint lightOn[NumLights] = {0};

// White directional light
LightProperties whiteDirLight = {
		DIRECTIONAL, //type
		{0.0f, 0.0f, 0.0f}, //pad
		vec4(0.1f, 0.1f, 0.1f, 1.0f), //ambient
		vec4(0.8f, 0.8f, 0.8f, 1.0f), //diffuse
		vec4(0.5f, 0.5f, 0.5f, 1.0f), //specular
		vec4(0.0f, 0.0f, 0.0f, 1.0f),  //position
		vec4(1.0f, -1.0f, -1.0f, 0.0f), //direction
		0.0f,   //cutoff
		0.0f,  //exponent
		{0.0f, 0.0f}  //pad2
};

/******************************************/
/*       INSERT (n) CODE HERE             */
/******************************************/
// TODO: Add red spotlight
LightProperties redSpotLight = {
	SPOT,
	{0.0f, 0.0f, 0.0f}, //pad
	vec4(0.1f, 0.0f, 0.0f, 1.0f), //ambient
	vec4(1.0f, 0.0f, 0.0f, 1.0f), //diffuse
	vec4(1.0f, 0.0f, 0.0f, 1.0f), //specular
	vec4(0.0f, 5.0f, 0.0f, 1.0f),  //position
	vec4(0.0f, -1.0f, 0.0f, 0.0f), //direction
	15.0f,   //cutoff
	0.1f,  //exponent
	{0.0f, 0.0f}  //pad2

};

// Red spotlight
LightProperties redTreeLight1 = {
		SPOT, //type
		{0.0f, 0.0f, 0.0f}, //pad
		vec4(0.0f, 0.0f, 0.0f, 1.0f), //ambient
		vec4(1.0f, 0.0f, 0.0f, 1.0f), //diffuse
		vec4(1.0f, 0.0f, 0.0f, 1.0f), //specular
		vec4(TREE_X+2.0f, BASE_HEIGHT+2*TREE_HEIGHT+STAR_HEIGHT, TREE_Z+2.0f, 1.0f),  //position
		vec4(-1.0f, 0.5f, -1.0f, 0.0f), //direction
		30.0f,   //cutoff
		0.0f,  //exponent
		{0.0f, 0.0f}  //pad2
};

// Green spotlight
LightProperties greenTreeLight1 = {
		SPOT, //type
		{0.0f, 0.0f, 0.0f}, //pad
		vec4(0.0f, 0.0f, 0.0f, 1.0f), //ambient
		vec4(0.0f, 1.0f, 0.0f, 1.0f), //diffuse
		vec4(0.0f, 1.0f, 0.0f, 1.0f), //specular
		vec4(TREE_X+2.0f, BASE_HEIGHT+2*TREE_HEIGHT+STAR_HEIGHT, TREE_Z+2.0f, 1.0f),  //position
		vec4(-1.0f, 0.5f, -1.0f, 0.0f), //direction
		30.0f,   //cutoff
		0.0f,  //exponent
		{0.0f, 0.0f}  //pad2
};

// Red spotlight 2
LightProperties redTreeLight2 = {
		SPOT, //type
		{0.0f, 0.0f, 0.0f}, //pad
		vec4(0.0f, 0.0f, 0.0f, 1.0f), //ambient
		vec4(1.0f, 0.0f, 0.0f, 1.0f), //diffuse
		vec4(1.0f, 0.0f, 0.0f, 1.0f), //specular
		vec4(TREE_X-2.0f, BASE_HEIGHT+2*TREE_HEIGHT+STAR_HEIGHT, TREE_Z+2.0f, 1.0f),  //position
		vec4(1.0f, 0.5f, -1.0f, 0.0f), //direction
		30.0f,   //cutoff
		0.0f,  //exponent
		{0.0f, 0.0f}  //pad2
};

// Green spotlight 2
LightProperties greenTreeLight2 = {
		SPOT, //type
		{0.0f, 0.0f, 0.0f}, //pad
		vec4(0.0f, 0.0f, 0.0f, 1.0f), //ambient
		vec4(0.0f, 1.0f, 0.0f, 1.0f), //diffuse
		vec4(0.0f, 1.0f, 0.0f, 1.0f), //specular
		vec4(TREE_X-2.0f, BASE_HEIGHT+2*TREE_HEIGHT+STAR_HEIGHT, TREE_Z+2.0f, 1.0f),  //position
		vec4(1.0f, 0.5f, -1.0f, 0.0f), //direction
		30.0f,   //cutoff
		0.0f,  //exponent
		{0.0f, 0.0f}  //pad2
};

void build_lights( ) {
	// Allocate Lights vector
	Lights.resize(NumLights);
	// Add lights to Lights vector
	Lights[WhiteDirLight] = whiteDirLight;
	lightOn[WhiteDirLight] = 1;
	if (mode > 5) {
		/******************************************/
		/*       INSERT (n) CODE HERE             */
		/******************************************/
		// TODO: Add red spotlight
		Lights[RedSpotLight] = redSpotLight;
		lightOn[RedSpotLight] = 1;

	}
	if (mode > 9) {
		Lights[RedTreeLight1] = redTreeLight1;
		Lights[GreenTreeLight1] = greenTreeLight1;
		Lights[RedTreeLight2] = redTreeLight2;
		Lights[GreenTreeLight2] = greenTreeLight2;
	}

    // Create uniform buffer for lights
    glGenBuffers(NumLightBuffers, LightBuffers);
    glBindBuffer(GL_UNIFORM_BUFFER, LightBuffers[LightBuffer]);
    glBufferData(GL_UNIFORM_BUFFER, Lights.size()*sizeof(LightProperties), Lights.data(), GL_STATIC_DRAW);
}
