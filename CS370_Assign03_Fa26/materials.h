// TODO: Add RedAcrylic constant
enum MaterialNames {Wood, SphereMaterial, CubeMaterial, OctMaterial, TorusMaterial, NumMaterials};
GLuint MaterialBuffers[NumMaterialBuffers];
vector<MaterialProperties> Materials;

// Create wood material
MaterialProperties wood = {
		vec4(0.66f, 0.46f, 0.16f, 1.0f), //ambient
		vec4(0.66f, 0.46f, 0.16f, 1.0f), //diffuse
		vec4(0.8f, 0.6f, 0.6f, 1.0f), //specular
		32.0f, //shininess
		{0.0f, 0.0f, 0.0f}  //pad
};

// TODO: Add materials
// Create sphere material
MaterialProperties sphere_material = {
	vec4(0.23f, 0.23f, 0.23f, 1.0f), // ambient
	vec4(0.55f, 0.55f, 0.55f, 1.0f), // diffuse
	vec4(0.95f, 0.95f, 0.95f, 1.0f), // specular
	256.0f,                          // shininess
	{0.0f, 0.0f, 0.0f}               // pad
};

//Create cube material
MaterialProperties cube_material = {
	vec4(0.25, 0.10f, 0.10f, 1.0f), // ambient
	vec4(0.75f, 0.1f, 0.1f, 1.0f), // diffuse
	vec4(0.01f, 0.01f, 0.01f, 1.0f), // specular
	670.0f,                          // shininess
	{0.0f, 0.0f, 0.0f}               // pad
};

//Create octahedron material
MaterialProperties oct_material = {
	vec4(0.10, 0.10, 0.23f, 0.5f), // ambient
	vec4(0.25f, 0.25f, 0.55f, 0.5f), // diffuse
	vec4(0.5f, 0.5f, 0.5f, 0.5f), // specular
	128.0f,                          // shininess
	{0.0f, 0.0f, 0.0f}               // pad
};

// Create sphere material
MaterialProperties torus_material = {
	vec4(0.2f, 0.1f, 0.2f, 1.0f), // ambient
	vec4(0.55f, 0.25f, 0.55f, 1.0f), // diffuse
	vec4(0.75f, 0.75f, 0.75f, 1.0f), // specular
	64.0f,                          // shininess
	{0.0f, 0.0f, 0.0f}               // pad
};

void build_materials( ) {
    // Allocate Materials vector
    Materials.resize(NumMaterials);
    // TODO: Add materials to Materials vector
    Materials[Wood] = wood;
	Materials[SphereMaterial] = sphere_material;
	Materials[CubeMaterial] = cube_material;
	Materials[OctMaterial] = oct_material;
	Materials[TorusMaterial] = torus_material;

    // Create uniform buffer for materials
    glGenBuffers(NumMaterialBuffers, MaterialBuffers);
    glBindBuffer(GL_UNIFORM_BUFFER, MaterialBuffers[MaterialBuffer]);
    glBufferData(GL_UNIFORM_BUFFER, Materials.size()*sizeof(MaterialProperties), Materials.data(), GL_STATIC_DRAW);
}
