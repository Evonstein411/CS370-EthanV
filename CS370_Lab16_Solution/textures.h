// Textures
enum Textures {MirrorTex, NumTextures};

GLuint TextureIDs[NumTextures];

void build_mirror(GLuint m_texid);;

void build_textures() {

    // Create textures and activate unit 0
    glGenTextures( NumTextures,  TextureIDs);
    glActiveTexture( GL_TEXTURE0 );

	// TODO: Create mirror texture
	build_mirror(MirrorTex);
}

void build_mirror(GLuint m_texid ) {
    // Bind mirror texture
    glBindTexture(GL_TEXTURE_2D, TextureIDs[m_texid]);
    // TODO: Create empty mirror texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ww, hh, 0, GL_RGBA, GL_FLOAT, NULL);

    // Set filters for no mipmapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

