// Textures
enum Textures {Earth, Moon, Space, NumTextures};

// Texture files
const char * earthFile = "../textures/earth.png";
const char * moonFile = "../textures/moon.bmp";
const char * spaceFile = "../textures/space.jpg";
GLuint TextureIDs[NumTextures];

void build_textures() {

    // Create textures and activate unit 0
    glGenTextures( NumTextures,  TextureIDs);
    glActiveTexture( GL_TEXTURE0 );

    // TODO: Load texture images
    load_texture(earthFile, TextureIDs[Earth], GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, GL_REPEAT, true, false);
    load_texture(moonFile, TextureIDs[Moon], GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, GL_REPEAT, true, false);
    load_texture(spaceFile, TextureIDs[Space], GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, GL_REPEAT, true, true);
}
