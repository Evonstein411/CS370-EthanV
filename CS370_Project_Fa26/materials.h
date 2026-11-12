enum MaterialNames {BRASS, BASE, FLOOR, NUMMATERIALS};
GLuint MaterialBuffers[NumMaterialBuffers];
vector<MaterialProperties> materials;
vector<MaterialProperties> dynamicMaterials;

float ambientR = 0.33f;
float ambientG = 0.22f;
float ambientB = 0.03f;
float ambientA = 1.0f;

float diffuseR = 0.78f;
float diffuseG = 0.57f;
float diffuseB = 0.11f;
float diffuseA = 1.0f;

float specularR = 0.99f;
float specularG = 0.91f;
float specularB = 0.81f;
float specularA = 1.0f;

float shininess = 27.8f;

MaterialProperties baseMaterial = {
	vec4(0.5f, 0.5f, 0.5f, 1.0f), //ambient
	vec4(0.5f, 0.5f, 0.5f, 1.0f), //diffuse
	vec4(0.5f, 0.5f, 0.5f, 1.0f), //specular
	25.0f, //shininess
	{0.0f, 0.0f, 0.0f}  //pad
};

MaterialProperties floorMat = {
	vec4(0.33f, 0.33f, 0.33f, 1.0f), //ambient
	vec4(0.33f, 0.33f, 0.33f, 1.0f), //diffuse
	vec4(0.33f, 0.33f, 0.33f, 1.0f), //specular
	10.0f, //shininess
	{0.0f, 0.0f, 0.0f}  //pad
};

// Create brass material
MaterialProperties brass = {
	vec4(0.33f, 0.22f, 0.03f, 1.0f), //ambient
	vec4(0.78f, 0.57f, 0.11f, 1.0f), //diffuse
	vec4(0.99f, 0.91f, 0.81f, 1.0f), //specular
	27.0f, //shininess
	{0.0f, 0.0f, 0.0f}  //pad
};

void build_materials() {
    // Allocate Materials vector
    materials.resize(NUMMATERIALS);
    // Add materials to Materials vector
    materials[BRASS] = brass;
	materials[FLOOR] = floorMat;
	materials[BASE] = baseMaterial;

    // Create uniform buffer for materials
    glGenBuffers(NumMaterialBuffers, MaterialBuffers);
    glBindBuffer(GL_UNIFORM_BUFFER, MaterialBuffers[MaterialBuffer]);
    glBufferData(GL_UNIFORM_BUFFER, materials.size()*sizeof(MaterialProperties), materials.data(), GL_STATIC_DRAW);
}

void build_dynamic_materials() {
	vector<MaterialProperties> all = materials;
	all.insert(all.end(), dynamicMaterials.begin(), dynamicMaterials.end());

	glBindBuffer(GL_UNIFORM_BUFFER, MaterialBuffers[MaterialBuffer]);
	glBufferData(GL_UNIFORM_BUFFER,
				 all.size() * sizeof(MaterialProperties),
				 all.data(),
				 GL_DYNAMIC_DRAW);
}
