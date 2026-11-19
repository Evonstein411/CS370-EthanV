#ifndef CS370_FA26_MESHCOMPONENT_H
#define CS370_FA26_MESHCOMPONENT_H

#include "component.h"
#include "appearance.h"
#include "../common/GLFWutils.h"

struct MeshComponent : Component {
    GLuint mesh = static_cast<GLuint>(-1);
    Appearance appearance;

    const char* get_type_name() const override { return "Mesh"; }

    void write(std::ostream& out) const override;
    bool read_field(const std::string& key, std::istream& iss) override;
    void on_loaded() override;
};

#endif