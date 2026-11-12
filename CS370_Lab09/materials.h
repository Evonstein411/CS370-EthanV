// TODO: Add RedPlastic constant
enum MaterialNames {Brass, NumMaterials};
GLuint MaterialBuffers[NumMaterialBuffers];
vector<MaterialProperties> materials;

// TODO: Create brass material


// TODO: Create red plastic material


void build_materials( ) {
    // Allocate Materials vector
    materials.resize(NumMaterials);
    // TODO: Add materials to Materials vector
    

    // Create uniform buffer for materials
    glGenBuffers(NumMaterialBuffers, MaterialBuffers);
    glBindBuffer(GL_UNIFORM_BUFFER, MaterialBuffers[MaterialBuffer]);
    glBufferData(GL_UNIFORM_BUFFER, materials.size()*sizeof(MaterialProperties), materials.data(), GL_STATIC_DRAW);
}
