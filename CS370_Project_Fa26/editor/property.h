#ifndef CS370_FA26_PROPERTY_H
#define CS370_FA26_PROPERTY_H

#include "textfield.h"
#include "label.h"
#include "panel.h"
#include <string>
#include <functional>

// Base so we can store mixed properties and call refresh/commit in a loop
struct Property {
    virtual ~Property() = default;
    virtual void refresh() = 0;
    virtual void commit() = 0;
};

// Binds a float to a TextField
struct FloatProperty : Property {
    float value = 0.0f;
    TextField<float>* field = nullptr;

    FloatProperty(Panel* panel, int col, int row,
                  const char* label, float initial = 0.0f)
        : value(initial) {
        if (label && label[0]) {
            auto* l = new Label(100.0f, 28.0f, label);
            panel->add(l, col, row);
            col++;
        }
        field = new TextField<float>(80.0f, 28.0f, "", 12, true, &value);
        panel->add(field, col, row);
    }

    void refresh() override { if (field) field->refresh(); }
    void commit()  override { if (field) field->commit(); }

    void set(float v) { value = v; refresh(); }
    float get()       { commit(); return value; }
};

// Binds a string
struct StringProperty : Property {
    std::string value;
    TextField<std::string>* field = nullptr;

    StringProperty(Panel* panel, int col, int row,
                   const char* label, std::string initial = "")
        : value(std::move(initial)) {
        if (label && label[0]) {
            auto* l = new Label(100.0f, 28.0f, label);
            panel->add(l, col, row);
            col++;
        }
        field = new TextField<std::string>(160.0f, 28.0f, "", 64, false, &value);
        panel->add(field, col, row);
    }

    void refresh() override { if (field) field->refresh(); }
    void commit()  override { if (field) field->commit(); }

    void set(const std::string& v) { value = v; refresh(); }
    const std::string& get()       { commit(); return value; }
};

// Convenience groups
struct Vec3Property {
    FloatProperty x, y, z;

    Vec3Property(Panel* panel, int row, const char* label)
        : x(panel, 1, row, "X:"),
          y(panel, 2, row, "Y:"),
          z(panel, 3, row, "Z:") {
        auto* l = new Label(110.0f, 28.0f, label);
        panel->add(l, 0, row, 1, 1, Alignment::STRETCH);
    }

    void set(const vmath::vec3& v) { x.set(v[0]); y.set(v[1]); z.set(v[2]); }
    vmath::vec3 get()              { return {x.get(), y.get(), z.get()}; }
    void refresh()                 { x.refresh(); y.refresh(); z.refresh(); }
    void commit()                  { x.commit();  y.commit();  z.commit(); }
};

struct Vec4Property {
    FloatProperty r, g, b, a;

    Vec4Property(Panel* panel, int row, const char* label)
        : r(panel, 1, row, "R:"),
          g(panel, 2, row, "G:"),
          b(panel, 3, row, "B:"),
          a(panel, 4, row, "A:") {
        auto* l = new Label(110.0f, 28.0f, label);
        panel->add(l, 0, row, 1, 1, Alignment::STRETCH);
    }

    void set(const vmath::vec4& v) { r.set(v[0]); g.set(v[1]); b.set(v[2]); a.set(v[3]); }
    vmath::vec4 get()              { return {r.get(), g.get(), b.get(), a.get()}; }
    void refresh()                 { r.refresh(); g.refresh(); b.refresh(); a.refresh(); }
    void commit()                  { r.commit();  g.commit();  b.commit();  a.commit(); }
};

#endif