// Textures
enum Textures {Carpet, Dirt, Blank, NumTextures};

// Texture files
const char * carpetFile = "../textures/carpet.jpg";
const char * dirtFile = "../textures/dirt.png";
const char * blankFile = "../textures/blank.png";
GLuint TextureIDs[NumTextures];

void build_textures( ) {

    // Create textures and activate unit 0
    glGenTextures( NumTextures,  TextureIDs);
    glActiveTexture( GL_TEXTURE0 );

    load_texture(carpetFile, TextureIDs[Carpet], GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, GL_REPEAT, true, false);
    load_texture(dirtFile, TextureIDs[Dirt], GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, GL_REPEAT, true, false);
    load_texture(blankFile, TextureIDs[Blank], GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, GL_REPEAT, true, false);
}
