//
// Created by ethan on 11/16/2026.
//

#ifndef CS370_FA26_LIGHTNODE_H
#define CS370_FA26_LIGHTNODE_H

#include <iosfwd>
#include <string>
#include "component.h"

struct LightComponent : Component {

    const char* get_type_name() const override { return "Light"; }

    void write(std::ostream& out) const override;
    bool read_field(const std::string& key, std::istream& iss) override;
    void on_loaded() override;
};


#endif //CS370_FA26_LIGHTNODE_H
