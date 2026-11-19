#ifndef CS370_FA26_COMPONENT_H
#define CS370_FA26_COMPONENT_H

#include <string>
#include <iosfwd>

struct Component {
    virtual ~Component() = default;

    virtual const char* get_type_name() const = 0;

    virtual void write(std::ostream& out) const {}
    virtual bool read_field(const std::string& key, std::istream& iss) { return false; }
    virtual void on_loaded() {}
};

#endif