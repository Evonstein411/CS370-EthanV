// Light constants
enum LightNames {WhitePointLight, RedSpotLight, CyanSpotLight, NumLights};
GLuint LightBuffers[NumLightBuffers];
vector<LightProperties> Lights;
GLint lightOn[NumLights] = {0};

/******************************************/
/*       INSERT (b) CODE HERE             */
/******************************************/
// TODO: Add white point light
LightProperties whitePointLight = {
    POINT,
    {0.0f, 0.0f, 0.0f},
    vec4(0.1f, 0.1f, 0.1f, 1.0f), //ambient
    vec4(1.0f, 1.0f, 1.0f, 1.0f), //diffuse
    vec4(1.0f, 1.0f, 1.0f, 1.0f), //specular
    vec4(-2.0f, 5.0f, -2.0f, 1.0f),  //position
};

/******************************************/
/*       INSERT (h) CODE HERE             */
/******************************************/
// TODO: Add red spotlight
LightProperties redSpotLight = {
    SPOT, //type
    {0.0f, 0.0f, 0.0f}, // pad
    vec4(0.1f, 0.0f, 0.0f, 1.0f), // ambient
    vec4(0.5f, 0.0f, 0.0f, 1.0f), // diffuse
    vec4(0.5f, 0.0f, 0.0f, 1.0f), // specular
    vec4(PLATE_X, 5.0f, PLATE_Z, 1.0f), // position
    vec4(0.0f, -1.0f, 0.0f, 0.0f), // direction
    20.0f,   // cutoff
    0.1f,    // exponent
    {0.0f, 0.0f} // pad2
};

/******************************************/
/*       INSERT (l) CODE HERE             */
/******************************************/
// TODO: Add cyan spotlight
LightProperties cyanSpotLight = {
    SPOT, //type
    {0.0f, 0.0f, 0.0f}, // pad
    vec4(0.1f, 0.0f, 0.0f, 1.0f), // ambient
    vec4(0.0f, 0.7f, 0.7f, 1.0f), // diffuse
    vec4(0.0f, 0.7f, 0.7f, 1.0f), // specular
    vec4(MUG_X, 4.0f, MUG_Z, 1.0f), // position
    vec4(0.0f, -1.0f, 0.0f, 0.0f), // direction
    10.0f,   // cutoff
    0.1f,    // exponent
    {0.0f, 0.0f} // pad2
};
 
void build_lights( ) {
	// Allocate Lights vector
	Lights.resize(NumLights);
    // Add lights to Lights vector
    if (mode > 0) {
        /******************************************/
        /*       INSERT (b) CODE HERE             */
        /******************************************/
        // TODO: Add white point light
        Lights[WhitePointLight] = whitePointLight;
        lightOn[WhitePointLight] = 1;

        
    }
    if (mode > 4) {
        /******************************************/
        /*       INSERT (h) CODE HERE             */
        /******************************************/
        // TODO: Add red spotlight
        Lights[RedSpotLight] = redSpotLight;
        lightOn[RedSpotLight] = 1;
    }
    if (mode > 6) {
        /******************************************/
        /*       INSERT (l) CODE HERE             */
        /******************************************/
        // TODO: Add cyan spotlight
        Lights[CyanSpotLight] = cyanSpotLight;
        lightOn[CyanSpotLight] = 1;

    }

    // Create uniform buffer for lights
    if (glIsBuffer(LightBuffers[LightBuffer])) {
        glDeleteBuffers(NumLightBuffers, LightBuffers);
    }
	// Create uniform buffer for lights
	glGenBuffers(NumLightBuffers, LightBuffers);
	glBindBuffer(GL_UNIFORM_BUFFER, LightBuffers[LightBuffer]);
	glBufferData(GL_UNIFORM_BUFFER, Lights.size()*sizeof(LightProperties), Lights.data(), GL_STATIC_DRAW);
}
