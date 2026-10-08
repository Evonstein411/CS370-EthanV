// TODO: Add RedAcrylic constant
enum MaterialNames {Brass, RedPlastic, WhitePlastic, NumMaterials};
GLuint MaterialBuffers[NumMaterialBuffers];
vector<MaterialProperties> Materials;

// Create brass material
MaterialProperties brass = {
		vec4(0.33f, 0.22f, 0.03f, 1.0f), //ambient
		vec4(0.78f, 0.57f, 0.11f, 1.0f), //diffuse
		vec4(0.99f, 0.91f, 0.81f, 1.0f), //specular
		27.8f, //shininess
		{0.0f, 0.0f, 0.0f}  //pad
};

// Create red plastic material
MaterialProperties redPlastic = {
		vec4(0.3f, 0.0f, 0.0f, 1.0f), //ambient
		vec4(0.6f, 0.0f, 0.0f, 1.0f), //diffuse
		vec4(0.8f, 0.6f, 0.6f, 1.0f), //specular
		32.0f, //shininess
		{0.0f, 0.0f, 0.0f}  //pad
};

// Create white plastic material
MaterialProperties whitePlastic = {
		vec4(1.0f, 1.0f, 1.0f, 1.0f), //ambient
		vec4(1.0f, 1.0f, 1.0f, 1.0f), //diffuse
		vec4(1.0f, 1.0f, 1.0f, 1.0f), //specular
		32.0f, //shininess
		{0.0f, 0.0f, 0.0f}  //pad
};

// TODO: Create red acrylic material


void build_materials( ) {
    // Allocate Materials vector
    Materials.resize(NumMaterials);
    // Add materials to Materials vector
    Materials[Brass] = brass;
    Materials[RedPlastic] = redPlastic;
    Materials[WhitePlastic] = whitePlastic;
    // TODO: Add red acrylic to Materials vector


    // Create uniform buffer for materials
    glGenBuffers(NumMaterialBuffers, MaterialBuffers);
    glBindBuffer(GL_UNIFORM_BUFFER, MaterialBuffers[MaterialBuffer]);
    glBufferData(GL_UNIFORM_BUFFER, Materials.size()*sizeof(MaterialProperties), Materials.data(), GL_STATIC_DRAW);
}
