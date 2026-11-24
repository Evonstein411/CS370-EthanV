#include "meshcomponent.h"

#include <iostream>
#include <string>

#include "rendering/geometry.h"
#include "../rendering/textures.h"


void MeshComponent::write(std::ostream& out) const {
    std::string meshName = "none";
    if (mesh != static_cast<GLuint>(-1) && mesh < meshNames.size())
        meshName = meshNames[mesh];
    out << "mesh " << meshName << "\n";

    out << "drawMode " << static_cast<int>(appearance.mode) << "\n";

    switch (appearance.mode) {
        case DrawMode::SolidColor:
            out << "color "
                << appearance.color[0] << " "
                << appearance.color[1] << " "
                << appearance.color[2] << " "
                << appearance.color[3] << "\n";
            break;

        case DrawMode::Material:
            out << "ambient "
                << appearance.material.ambient[0] << " "
                << appearance.material.ambient[1] << " "
                << appearance.material.ambient[2] << " "
                << appearance.material.ambient[3] << "\n";
            out << "diffuse "
                << appearance.material.diffuse[0] << " "
                << appearance.material.diffuse[1] << " "
                << appearance.material.diffuse[2] << " "
                << appearance.material.diffuse[3] << "\n";
            out << "specular "
                << appearance.material.specular[0] << " "
                << appearance.material.specular[1] << " "
                << appearance.material.specular[2] << " "
                << appearance.material.specular[3] << "\n";
            out << "shininess " << appearance.material.shininess << "\n";
            break;

        case DrawMode::Texture:
            out << "texture " << appearance.textureName << "\n";
            break;

        default:
            break;
    }
}

bool MeshComponent::read_field(const std::string& key, std::istream& iss) {
    if (key == "mesh") {
        std::string meshName;
        iss >> meshName;
        if (const int idx = find_mesh(meshName); idx >= 0)
            mesh = static_cast<GLuint>(idx);
        return true;
    }

    if (key == "drawMode") {
        int m = 0;
        iss >> m;
        appearance.mode = static_cast<DrawMode>(m);
        return true;
    }

    if (key == "color") {
        iss >> appearance.color[0] >> appearance.color[1]
            >> appearance.color[2] >> appearance.color[3];
        return true;
    }

    if (key == "ambient") {
        iss >> appearance.material.ambient[0] >> appearance.material.ambient[1]
            >> appearance.material.ambient[2] >> appearance.material.ambient[3];
        return true;
    }
    if (key == "diffuse") {
        iss >> appearance.material.diffuse[0] >> appearance.material.diffuse[1]
            >> appearance.material.diffuse[2] >> appearance.material.diffuse[3];
        return true;
    }
    if (key == "specular") {
        iss >> appearance.material.specular[0] >> appearance.material.specular[1]
            >> appearance.material.specular[2] >> appearance.material.specular[3];
        return true;
    }
    if (key == "shininess") {
        iss >> appearance.material.shininess;
        return true;
    }

    if (key == "texture") {
        iss >> appearance.textureName;
        return true;
    }

    return false;
}

void MeshComponent::on_loaded() {
    if (appearance.mode == DrawMode::Texture && !appearance.textureName.empty()) {
        const int idx = find_texture(appearance.textureName);
        appearance.texture = (idx >= 0) ? TextureIDs[idx] : TextureIDs[0];
    }
    appearance.build(mesh);
}