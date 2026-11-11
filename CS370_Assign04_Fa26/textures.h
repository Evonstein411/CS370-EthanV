// Textures
enum Textures {YCP, Shirt, Face, Basketball, Wood, NumTextures};

// Texture files
const char * ycpFile = "../textures/ycp.png";
const char * shirtFile = "../textures/shirt_z.png";
const char * faceFile = "../textures/face.png";
const char * bballFile = "../textures/bball.png";
const char * woodFile = "../textures/wood.png";
GLuint TextureIDs[NumTextures];

void build_textures() {

    // Create textures and activate unit 0
    glGenTextures( NumTextures,  TextureIDs);
    glActiveTexture( GL_TEXTURE0 );

    // Load texture images
    load_texture(ycpFile, TextureIDs[YCP], GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, GL_REPEAT, true, true);
    load_texture(shirtFile, TextureIDs[Shirt], GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, GL_REPEAT, true, true);
    load_texture(faceFile, TextureIDs[Face], GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, GL_REPEAT, true, true);
    load_texture(bballFile, TextureIDs[Basketball], GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, GL_REPEAT, true, true);
    load_texture(woodFile, TextureIDs[Wood], GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, GL_REPEAT, true, true);
}
