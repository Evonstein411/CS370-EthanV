#define STB_TRUETYPE_IMPLEMENTATION
#include <string>
#include <filesystem>
#include "../common/stb_truetype.h"

//Files
const char * fontFile = "../fonts/Roboto-Regular.ttf";
namespace fs = std::filesystem;
// Textures
enum Textures {Blank};

vector<string> textureNames;
vector<GLuint> TextureIDs;
GLuint fontTextureID = 0;

stbtt_bakedchar glyphs[96];

//function forward decs
void build_font();
void build_textures();
void scan_textures(const std::string& folder = "../textures");


void scan_textures(const std::string& folder) {
    textureNames.clear();
    TextureIDs.clear();

    // Blank always at index 0
    textureNames.push_back("Blank");
    GLuint blankID;
    glGenTextures(1, &blankID);
    load_texture("../textures/blank.png", blankID, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, GL_REPEAT, true, false);
    TextureIDs.push_back(blankID);

    //return if not a folder/does not exist
     if (!fs::exists(folder) || !fs::is_directory(folder))
        return;

    //go through each file
    for (const auto& entry : fs::directory_iterator(folder)) {
        if (!entry.is_regular_file()) continue;

        fs::path p = entry.path();
        std::string ext = p.extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        if (ext != ".png") continue;

        std::string stem = p.stem().string();
        if (stem == "blank") continue;   // skip specials

        GLuint id;
        glGenTextures(1, &id);
        load_texture(p.string().c_str(), id, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, GL_REPEAT, true, false);
        textureNames.push_back(stem);
        TextureIDs.push_back(id);

    }
}



void build_textures( ) {

    glActiveTexture(GL_TEXTURE0);
    scan_textures();

    // Font remains a special baked atlas (not in the selectable list)
    build_font();


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

    glBindTexture(GL_TEXTURE_2D, fontTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, W, H, 0, GL_RED, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    delete[] ttf;
    delete[] pixels;
}

