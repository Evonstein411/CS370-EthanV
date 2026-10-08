// Textures
enum Textures {Earth, NumTextures};

// Texture files
const char * earthFile = "../textures/earth.png";
GLuint TextureIDs[NumTextures];

void build_textures() {

    // Create textures and activate unit 0
    glGenTextures( NumTextures,  TextureIDs);
    glActiveTexture( GL_TEXTURE0 );

    // Load texture images
    load_texture(earthFile, TextureIDs[Earth], GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, GL_REPEAT, true, false);
}
