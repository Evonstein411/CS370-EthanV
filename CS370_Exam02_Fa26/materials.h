// Material constants
enum MaterialNames {Steel, Copper, Dough, BlueAcrylic, GreenGlass, CoffeeBlack, NumMaterials};
GLuint MaterialBuffers[NumMaterialBuffers];
vector<MaterialProperties> materials;

// Create grey plastic material
MaterialProperties steel = {
		vec4(0.1f, 0.1f, 0.1f, 1.0f), //ambient
		vec4(0.6f, 0.6f, 0.6f, 1.0f), //diffuse
		vec4(0.8f, 0.8f, 0.8f, 1.0f), //specular
		10.0f, //shininess
		{0.0f, 0.0f, 0.0f}  //pad
};

/******************************************/
/*       INSERT (c) CODE HERE             */
/******************************************/
// TODO: Create copper material
MaterialProperties copper = {
    vec4(0.19f, 0.07f, 0.02f, 1.0f), //ambient
    vec4(0.7f, 0.27f, 0.08f, 1.0f), //diffuse
    vec4(0.26f, 0.14f, 0.09f, 1.0f), //specular
    12.8f, //shininess
    {0.0f, 0.0f, 0.0f}  //pad
};

/******************************************/
/*       INSERT (e) CODE HERE             */
/******************************************/
// TODO: Create dough material
MaterialProperties dough = {
    vec4(0.3f, 0.3f, 0.02f, 1.0f), //ambient
    vec4(0.55f, 0.55f, 0.05f, 1.0f), //diffuse
    vec4(0.1f, 0.1f, 0.05f, 1.0f), //specular
    40.0f, //shininess
    {0.0f, 0.0f, 0.0f}  //pad
};

/******************************************/
/*       INSERT (i) CODE HERE             */
/******************************************/
// TODO: Create blue acrylic (translucent) material
// TODO: Create green glass (translucent) material
MaterialProperties blueAcrylic = {
    vec4(0.02f, 0.02f, 0.3f, 0.6f), //ambient
    vec4(0.05f, 0.15f, 0.55f, 1.0f), //diffuse
    vec4(0.05f, 0.05f, 0.1f, 1.0f), //specular
    15.0f, //shininess
    {0.0f, 0.0f, 0.0f}  //pad
};

MaterialProperties greenGlass = {
    vec4(0.02f, 0.03f, 0.02f, 0.8f), //ambient
    vec4(0.05f, 0.55f, 0.15f, 1.0f), //diffuse
    vec4(0.05f, 0.1f, 0.05f, 1.0f), //specular
    15.0f, //shininess
    {0.0f, 0.0f, 0.0f}  //pad
};

/******************************************/
/*       INSERT (n) CODE HERE             */
/******************************************/
// TODO: Create coffeeBlack (translucent) material
MaterialProperties coffeeBlack = {
    vec4(0.01f, 0.01f, 0.01f, 0.75f), //ambient
    vec4(0.05f, 0.05f, 0.05f, 1.0f), //diffuse
    vec4(0.05f, 0.05f, 0.05f, 1.0f), //specular
    15.0f, //shininess
    {0.0f, 0.0f, 0.0f}  //pad
};

void build_materials( ) {
    // Allocate Materials vector
    materials.resize(NumMaterials);
    // Add materials to Materials vector
    materials[Steel] = steel;
    if (mode >= 2) {
        /******************************************/
        /*       INSERT (c) CODE HERE             */
        /******************************************/
        materials[Copper] = copper;
    }

    if (mode >= 3) {
        /******************************************/
        /*       INSERT (e) CODE HERE             */
        /******************************************/
        materials[Dough] = dough;
    }

    if (mode >= 7) {
        /******************************************/
        /*       INSERT (i) CODE HERE             */
        /******************************************/
        materials[BlueAcrylic]= blueAcrylic;
        materials[GreenGlass] = greenGlass;

    }

    if (mode >= 9) {
        /******************************************/
        /*       INSERT (n) CODE HERE             */
        /******************************************/
		materials[CoffeeBlack] = coffeeBlack;
       
    }

    // Create uniform buffer for materials
    if (glIsBuffer(MaterialBuffers[MaterialBuffer])) {
        glDeleteBuffers(NumMaterialBuffers, MaterialBuffers);
    }
    // Create uniform buffer for materials
    glGenBuffers(NumMaterialBuffers, MaterialBuffers);
    glBindBuffer(GL_UNIFORM_BUFFER, MaterialBuffers[MaterialBuffer]);
    glBufferData(GL_UNIFORM_BUFFER, materials.size()*sizeof(MaterialProperties), materials.data(), GL_STATIC_DRAW);
}
