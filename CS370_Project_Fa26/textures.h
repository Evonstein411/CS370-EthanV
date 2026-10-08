#define STB_TRUETYPE_IMPLEMENTATION
#include "../common/stb_truetype.h"


// Textures
enum Textures {Blank, Font, NumTextures};
// Texture files
const char * blankFile = "../textures/blank.png";
const char * fontFile = "../textures/Roboto-Regular.ttf";

GLuint TextureIDs[NumTextures];
stbtt_bakedchar glyphs[96];

void build_font();
void build_textures();

void build_textures( ) {

    // Create textures and activate unit 0
    glGenTextures( NumTextures,  TextureIDs);
    build_font();
    glActiveTexture( GL_TEXTURE0 );

    load_texture(blankFile, TextureIDs[Blank], GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, GL_REPEAT, true, false);


}

void build_font() {
    FILE* f = fopen(fontFile, "rb");
    if (!f) { fprintf(stderr, "ERROR: font file missing\n"); return; }
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);
    unsigned char* ttf = new unsigned char[size];
    fread(ttf, 1, size, f);
    fclose(f);

    const int W = 512, H = 512;
    unsigned char* pixels = new unsigned char[W * H];
    memset(pixels, 0, W * H);
    stbtt_BakeFontBitmap(ttf, 0, 32.0f, pixels, W, H, 32, 96, glyphs);

    glBindTexture(GL_TEXTURE_2D, TextureIDs[Font]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, W, H, 0, GL_RED, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    delete[] ttf;
    delete[] pixels;
}

