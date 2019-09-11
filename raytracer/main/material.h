#ifndef MATERIAL_H
#define MATERIAL_H

#include <optional>
#include "color.h"
#include "pattern.h"

struct Material {
    Color color = Color(1, 1, 1);
    float ambient   = 0.1f;
    float diffuse   = 0.9f;
    float specular  = 0.9f;
    float shininess = 200.0f;
    std::optional<Pattern> pattern;

    bool operator ==(const Material& m) const {
        if (!(color == m.color)) return false;
        if (!equal(ambient, m.ambient)) return false;
        if (!equal(diffuse, m.diffuse)) return false;
        if (!equal(specular, m.specular)) return false;
        if (!equal(shininess, m.shininess)) return false;

        return true;
    }
};

#endif //MATERIAL_H
