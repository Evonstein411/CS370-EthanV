#ifndef MATERIALS_H
#define MATERIALS_H

#include <vector>

//copy of materialProperties from lighting.h change later for more functionality
struct Material {
	vmath::vec4 ambient;
	vmath::vec4 diffuse;
	vmath::vec4 specular;
	GLfloat shininess;
	GLfloat pad[3];
};


enum MaterialBuffer_IDs {MaterialBuffer, NumMaterialBuffers};
enum MaterialNames {BRASS, BASE, FLOOR, NUMMATERIALS};

inline GLuint MaterialBuffers[NumMaterialBuffers];
inline std::vector<Material> materials;
inline std::vector<Material> dynamicMaterials;

inline float ambientR = 0.33f;
inline float ambientG = 0.22f;
inline float ambientB = 0.03f;
inline float ambientA = 1.0f;

inline float diffuseR = 0.78f;
inline float diffuseG = 0.57f;
inline float diffuseB = 0.11f;
inline float diffuseA = 1.0f;

inline float specularR = 0.99f;
inline float specularG = 0.91f;
inline float specularB = 0.81f;
inline float specularA = 1.0f;

inline float shininess = 27.8f;

inline Material baseMaterial = {
	vmath::vec4(0.5f, 0.5f, 0.5f, 1.0f), //ambient
	vmath::vec4(0.5f, 0.5f, 0.5f, 1.0f), //diffuse
	vmath::vec4(0.5f, 0.5f, 0.5f, 1.0f), //specular
	25.0f, //shininess
	{0.0f, 0.0f, 0.0f}  //pad
};

inline Material floorMat = {
	vmath::vec4(0.33f, 0.33f, 0.33f, 1.0f), //ambient
	vmath::vec4(0.33f, 0.33f, 0.33f, 1.0f), //diffuse
	vmath::vec4(0.33f, 0.33f, 0.33f, 1.0f), //specular
	10.0f, //shininess
	{0.0f, 0.0f, 0.0f}  //pad
};

// Create brass material
inline Material brass = {
	vmath::vec4(0.33f, 0.22f, 0.03f, 1.0f), //ambient
	vmath::vec4(0.78f, 0.57f, 0.11f, 1.0f), //diffuse
	vmath::vec4(0.99f, 0.91f, 0.81f, 1.0f), //specular
	27.0f, //shininess
	{0.0f, 0.0f, 0.0f}  //pad
};

inline void build_materials() {
    // Allocate Materials vector
    materials.resize(NUMMATERIALS);
    // Add materials to Materials vector
    materials[BRASS] = brass;
	materials[FLOOR] = floorMat;
	materials[BASE] = baseMaterial;

    // Create uniform buffer for materials
    glGenBuffers(NumMaterialBuffers, MaterialBuffers);
    glBindBuffer(GL_UNIFORM_BUFFER, MaterialBuffers[MaterialBuffer]);
    glBufferData(GL_UNIFORM_BUFFER, materials.size()*sizeof(Material), materials.data(), GL_STATIC_DRAW);
}

inline void build_dynamic_materials() {
	std::vector<Material> all = materials;
	all.insert(all.end(), dynamicMaterials.begin(), dynamicMaterials.end());

	glBindBuffer(GL_UNIFORM_BUFFER, MaterialBuffers[MaterialBuffer]);
	glBufferData(GL_UNIFORM_BUFFER,
				 all.size() * sizeof(Material),
				 all.data(),
				 GL_DYNAMIC_DRAW);
}

#endif //MATERIALS_H