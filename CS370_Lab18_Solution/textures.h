// Textures
enum Textures {Carpet, Dirt, Blank, Golf, GolfNormFlat, GolfNormOut, GolfNormIn, NumTextures};
// Texture files
const char * carpetFile = "../textures/carpet.jpg";
const char * dirtFile = "../textures/dirt.png";
const char * blankFile = "../textures/blank.png";
const char * golfFile = "../textures/golf4.png";
const char * golfNormFlatFile = "../textures/golfNormFlat.png";
const char * golfNormOutFile = "../textures/golfNormOut.png";
const char * golfNormInFile = "../textures/golfNormIn.png";
GLuint TextureIDs[NumTextures];

void build_textures( ) {

    // Create textures and activate unit 0
    glGenTextures( NumTextures,  TextureIDs);
    glActiveTexture( GL_TEXTURE0 );

    load_texture(carpetFile, TextureIDs[Carpet], GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, GL_REPEAT, true, false);
    load_texture(dirtFile, TextureIDs[Dirt], GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, GL_REPEAT, true, false);
    load_texture(blankFile, TextureIDs[Blank], GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, GL_REPEAT, true, false);
    load_texture(golfFile, TextureIDs[Golf], GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, GL_REPEAT, true, false);
    load_texture(golfNormFlatFile, TextureIDs[GolfNormFlat], GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, GL_REPEAT, true, false);
    
    // TODO: Load additional golf ball normal maps into textures
    load_texture(golfNormInFile, TextureIDs[GolfNormIn], GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, GL_REPEAT, true, false);
    load_texture(golfNormOutFile, TextureIDs[GolfNormOut], GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, GL_REPEAT, true, false);
}
