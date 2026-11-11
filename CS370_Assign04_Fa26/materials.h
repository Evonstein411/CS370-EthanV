enum MaterialNames {Skin, NumMaterials};
GLuint MaterialBuffers[NumMaterialBuffers];
vector<MaterialProperties> Materials;

// Create skin material
MaterialProperties skin = {
		vec4(0.33f, 0.22f, 0.03f, 1.0f), //ambient
		vec4(0.78f, 0.57f, 0.11f, 1.0f), //diffuse
		vec4(0.99f, 0.91f, 0.81f, 1.0f), //specular
		10.0f, //shininess
		{0.0f, 0.0f, 0.0f}  //pad
};

// TODO: Add additional materials

void build_materials( ) {
    // Allocate Materials vector
    Materials.resize(NumMaterials);
    // TODO: Add materials to Materials vector
    Materials[Skin] = skin;


    // Create uniform buffer for materials
    glGenBuffers(NumMaterialBuffers, MaterialBuffers);
    glBindBuffer(GL_UNIFORM_BUFFER, MaterialBuffers[MaterialBuffer]);
    glBufferData(GL_UNIFORM_BUFFER, Materials.size()*sizeof(MaterialProperties), Materials.data(), GL_STATIC_DRAW);
}
