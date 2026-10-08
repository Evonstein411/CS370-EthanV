enum MaterialNames {RedPlastic, NumMaterials};
GLuint MaterialBuffers[NumMaterialBuffers];
vector<MaterialProperties> Materials;

// Create red plastic material
MaterialProperties redPlastic = {
		vec4(0.3f, 0.0f, 0.0f, 1.0f), //ambient
		vec4(0.6f, 0.0f, 0.0f, 1.0f), //diffuse
		vec4(0.8f, 0.6f, 0.6f, 1.0f), //specular
		32.0f, //shininess
		{0.0f, 0.0f, 0.0f}  //pad
};

void build_materials( ) {
    // Allocate Materials vector
    Materials.resize(NumMaterials);
    // Add materials to Materials vector
    Materials[RedPlastic] = redPlastic;

    // Create uniform buffer for materials
    glGenBuffers(NumMaterialBuffers, MaterialBuffers);
    glBindBuffer(GL_UNIFORM_BUFFER, MaterialBuffers[MaterialBuffer]);
    glBufferData(GL_UNIFORM_BUFFER, Materials.size()*sizeof(MaterialProperties), Materials.data(), GL_STATIC_DRAW);
}
