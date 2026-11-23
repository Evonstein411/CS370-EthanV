// Textures
enum Textures {Snow, Tree, Grinch, Skirt, TurkeyTex, TableCloth, WrappingPaper, NumTextures};

// Texture files
const char * snowFile = "../textures/snow.png";
const char * treeFile = "../textures/tree.png";
const char * grinchFile = "../textures/grinch.png";
const char * tableClothFile = "../textures/tartan2.jpg";
const char * skirtFile = "../textures/skirt.jpg";
const char * turkeyTexFile = "../textures/turkey.jpg";
const char * wrappingPaperFile = "../textures/present.png";
GLuint TextureIDs[NumTextures];

void build_textures() {

    // Create textures and activate unit 0
    glGenTextures( NumTextures,  TextureIDs);
    glActiveTexture( GL_TEXTURE0 );

    load_texture(snowFile, TextureIDs[Snow], GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, GL_REPEAT, true, true);
    load_texture(treeFile, TextureIDs[Tree], GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, GL_REPEAT, true, true);
    load_texture(grinchFile, TextureIDs[Grinch], GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, GL_REPEAT, true, false);
    load_texture(skirtFile, TextureIDs[Skirt], GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, GL_REPEAT, true, true);
    load_texture(turkeyTexFile, TextureIDs[TurkeyTex], GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, GL_REPEAT, true, true);
    load_texture(tableClothFile, TextureIDs[TableCloth], GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, GL_REPEAT, true, true);
    load_texture(wrappingPaperFile, TextureIDs[WrappingPaper], GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, GL_REPEAT, true, true);
}
