// TODO: Add RedAcrylic constant
enum MaterialNames {Brass, RedPlastic, WhitePlastic, RedAcrylic, NumMaterials};
GLuint MaterialBuffers[NumMaterialBuffers];
vector<MaterialProperties> materials;

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
MaterialProperties redAcrylic = {
		vec4(0.3f, 0.0f, 0.0f, 0.5f), //ambient
		vec4(0.6f, 0.0f, 0.0f, 0.5f), //diffuse
		vec4(0.8f, 0.6f, 0.6f, 0.5f), //specular
		32.0f, //shininess
		{0.0f, 0.0f, 0.0f}  //pad
};


void build_materials( ) {
    // Allocate Materials vector
    materials.resize(NumMaterials);
    // Add materials to Materials vector
    materials[Brass] = brass;
    materials[RedPlastic] = redPlastic;
    materials[WhitePlastic] = whitePlastic;
    // TODO: Add red acrylic to Materials vector
    materials[RedAcrylic] = redAcrylic;

    // Create uniform buffer for materials
    glGenBuffers(NumMaterialBuffers, MaterialBuffers);
    glBindBuffer(GL_UNIFORM_BUFFER, MaterialBuffers[MaterialBuffer]);
    glBufferData(GL_UNIFORM_BUFFER, materials.size()*sizeof(MaterialProperties), materials.data(), GL_STATIC_DRAW);
}
