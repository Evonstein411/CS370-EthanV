// Textures
enum ShadowTextures {ShadowTex1, NumShadowTextures};
enum ShadowBuffers {ShadowBuffer1, NumShadowBuffers};

GLuint ShadowTextureIDs[NumShadowTextures];
GLuint ShadowBufferIDs[NumShadowBuffers];

void build_shadows(GLuint s_buff, GLuint s_texid);;

void build_textures() {

    // Create textures and activate unit 0
    glGenTextures( NumShadowTextures,  ShadowTextureIDs);
    glActiveTexture( GL_TEXTURE0 );

	// TODO: Create mirror texture
	build_shadows(ShadowBuffer1, ShadowTex1);
}

void build_shadows(GLuint s_buff, GLuint s_texid) {
    // Generate new framebuffer and corresponding texture for storing shadow distances
    glGenFramebuffers(1, &ShadowBufferIDs[s_buff]);
    // Bind shadow texture and only store depth value
    glBindTexture(GL_TEXTURE_2D, ShadowTextureIDs[s_texid]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindFramebuffer(GL_FRAMEBUFFER, ShadowBufferIDs[s_buff]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ShadowTextureIDs[s_texid], 0);
    // Buffer is not actually drawn into since only for creating shadow texture
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
