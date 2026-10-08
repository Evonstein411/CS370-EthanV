enum MaterialNames {Brass, Floor, NumMaterials};
GLuint MaterialBuffers[NumMaterialBuffers];
vector<MaterialProperties> Materials;

float ambient_r = 0.33f;
float ambient_g = 0.22f;
float ambient_b = 0.03f;
float ambient_a = 1.0f;



// Create brass material
MaterialProperties brass = {
		vec4(ambient_r, ambient_g, ambient_b, ambient_a), //ambient
		vec4(0.78f, 0.57f, 0.11f, 1.0f), //diffuse
		vec4(0.99f, 0.91f, 0.81f, 1.0f), //specular
		27.8f, //shininess
		{0.0f, 0.0f, 0.0f}  //pad
};

MaterialProperties floormat = {
	vec4(0.33f, 0.33f, 0.33f, 1.0f), //ambient
	vec4(0.33f, 0.33f, 0.33f, 1.0f), //diffuse
	vec4(0.33f, 0.33f, 0.33f, 1.0f), //specular
	10.0f, //shininess
	{0.0f, 0.0f, 0.0f}  //pad
};


void build_materials() {
    // Allocate Materials vector
    Materials.resize(NumMaterials);
    // Add materials to Materials vector
    Materials[Brass] = brass;
	Materials[Floor] = floormat;

    // Create uniform buffer for materials
    glGenBuffers(NumMaterialBuffers, MaterialBuffers);
    glBindBuffer(GL_UNIFORM_BUFFER, MaterialBuffers[MaterialBuffer]);
    glBufferData(GL_UNIFORM_BUFFER, Materials.size()*sizeof(MaterialProperties), Materials.data(), GL_STATIC_DRAW);
}
