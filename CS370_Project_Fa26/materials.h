enum MaterialNames {Brass, Base, Floor, NumMaterials};
GLuint MaterialBuffers[NumMaterialBuffers];
vector<MaterialProperties> Materials;

vector<MaterialProperties> DynamicMaterials;
GLuint DynamicMaterialBuffers[1];

float ambient_r = 0.33f;
float ambient_g = 0.22f;
float ambient_b = 0.03f;
float ambient_a = 1.0f;

float diffuse_r = 0.78f;
float diffuse_g = 0.57f;
float diffuse_b = 0.11f;
float diffuse_a = 1.0f;

float specular_r = 0.99f;
float specular_g = 0.91f;
float specular_b = 0.81f;
float specular_a = 1.0f;

float shininess = 27.8f;

MaterialProperties baseMaterial = {
	vec4(1.0f, 1.0f, 1.0f, 1.0f), //ambient
	vec4(0.5f, 0.5f, 0.5f, 1.0f), //diffuse
	vec4(0.5f, 0.5f, 0.5f, 1.0f), //specular
	10.0f, //shininess
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
	// Create brass material
	MaterialProperties brass = {
		vec4(ambient_r, ambient_g, ambient_b, ambient_a), //ambient
		vec4(diffuse_r, diffuse_g, diffuse_b, diffuse_a), //diffuse
		vec4(specular_r, specular_g, specular_b, specular_a), //specular
		shininess, //shininess
		{0.0f, 0.0f, 0.0f}  //pad
	};

    // Allocate Materials vector
    Materials.resize(NumMaterials);
    // Add materials to Materials vector
    Materials[Brass] = brass;
	Materials[Floor] = floormat;
	Materials[Base] = baseMaterial;

    // Create uniform buffer for materials
    glGenBuffers(NumMaterialBuffers, MaterialBuffers);
    glBindBuffer(GL_UNIFORM_BUFFER, MaterialBuffers[MaterialBuffer]);
    glBufferData(GL_UNIFORM_BUFFER, Materials.size()*sizeof(MaterialProperties), Materials.data(), GL_STATIC_DRAW);
}

void build_dynamic_materials() {

	glGenBuffers(DynamicMaterials.size(), DynamicMaterialBuffers);
	glBindBuffer(GL_UNIFORM_BUFFER, DynamicMaterialBuffers[MaterialBuffer]);
	glBufferData(GL_UNIFORM_BUFFER, DynamicMaterials.size()*sizeof(MaterialProperties), DynamicMaterials.data(), GL_STATIC_DRAW);


}
