enum MaterialNames {WhitePlastic, GreenPlastic, Silver, NumMaterials};
GLuint MaterialBuffers[NumMaterialBuffers];
vector<MaterialProperties> Materials;

// Create white plastic material
MaterialProperties whitePlastic = {vec4(0.2f, 0.2f, 0.2f, 1.0f), //ambient
								   vec4(1.0f, 1.0f, 1.0f, 1.0f), //diffuse
								   vec4(1.0f, 1.0f, 1.0f, 1.0f), //specular
								   12.0f, //shininess
								   {0.0f, 0.0f, 0.0f}  //pad
};

// Create green plastic material
MaterialProperties greenPlastic = {vec4(0.0f, 0.3f, 0.0f, 1.0f), //ambient
								  vec4(0.0f, 0.3f, 0.0f, 1.0f), //diffuse
								  vec4(0.6f, 0.8f, 0.6f, 1.0f), //specular
								  32.0f, //shininess
								  {0.0f, 0.0f, 0.0f}  //pad
};

// Create silver material
MaterialProperties silver = {vec4(0.0f, 0.0f, 0.0f, 1.0f), //ambient
								 vec4(0.7843f, 0.7843f, 0.7843f, 1.0f), //diffuse
								 vec4(0.63f, 0.63f, 0.63f, 1.0f), //specular
								 70.0f, //shininess
								 {0.0f, 0.0f, 0.0f}  //pad
};

void build_materials( ) {
    // Allocate Materials vector
    Materials.resize(NumMaterials);
    // TODO: Add materials to Materials vector
    Materials[WhitePlastic] = whitePlastic;
    Materials[GreenPlastic] = greenPlastic;
    Materials[Silver] = silver;

    // Create uniform buffer for materials
    glGenBuffers(NumMaterialBuffers, MaterialBuffers);
    glBindBuffer(GL_UNIFORM_BUFFER, MaterialBuffers[MaterialBuffer]);
    glBufferData(GL_UNIFORM_BUFFER, Materials.size()*sizeof(MaterialProperties), Materials.data(), GL_STATIC_DRAW);
}
