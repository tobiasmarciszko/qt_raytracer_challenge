#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"
#include "pattern.h"

struct Material {
    Color color = Color(1, 1, 1);
    float ambient   = 0.1;
    float diffuse   = 0.9;
    float specular  = 0.9;
    float shininess = 200.0;
    float reflective = 0.0;
    float transparency = 0.0;
    float refractive_index = 1.0;

    std::shared_ptr<Pattern> pattern_ptr = nullptr;

    bool operator ==(const Material& m) const {
        if (!(color == m.color)) return false;
        if (!equal(ambient, m.ambient)) return false;
        if (!equal(diffuse, m.diffuse)) return false;
        if (!equal(specular, m.specular)) return false;
        if (!equal(shininess, m.shininess)) return false;
        if (!equal(reflective, m.reflective)) return false;
        if (!equal(transparency, m.transparency)) return false;
        if (!equal(refractive_index, m.refractive_index)) return false;

        return true;
    }
};

#endif //MATERIAL_H
