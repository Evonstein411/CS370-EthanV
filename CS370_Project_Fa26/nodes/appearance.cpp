#include "appearance.h"
#include "../rendering/geometry.h"    // numVertices, etc.
#include "../rendering/materials.h"   // dynamicMaterials, build_dynamic_materials
#include "../rendering/drawobjects.h" // ColorBuffers if declared there

#include <vector>

void Appearance::build(GLuint mesh) {
    destroy();   // clear any previous resources

    switch (mode) {
        case DrawMode::SolidColor: {
            if (mesh == static_cast<GLuint>(-1)) break;

            // create a solid color buffer for this mesh
            std::vector<vmath::vec4> cols(numVertices[mesh], color);
            glGenBuffers(1, &colorBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
            glBufferData(GL_ARRAY_BUFFER,
                         cols.size() * sizeof(vmath::vec4),
                         cols.data(),
                         GL_STATIC_DRAW);
            break;
        }

        case DrawMode::Material: {
            // register with dynamic materials and rebuild the UBO
            dynamicMaterials.push_back(material);
            materialIndex = static_cast<int>(materials.size() + dynamicMaterials.size() - 1);
            build_dynamic_materials();
            break;
        }

        case DrawMode::Texture:
            // texture ID is already resolved in MeshComponent::on_loaded
            break;

        default:
            break;
    }
}

void Appearance::destroy() {
    if (colorBuffer) {
        glDeleteBuffers(1, &colorBuffer);
        colorBuffer = 0;
    }
    materialIndex = -1;
}