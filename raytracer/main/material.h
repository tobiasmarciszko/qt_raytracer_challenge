#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"
#include "pattern.h"

struct Material {

    Material() = default;

    Material(Color _color,
             float _ambient,
             float _diffuse,
             float _specular,
             float _shininess,
             float _reflective,
             float _transparency,
             float _refractive_index,
             std::shared_ptr<Pattern> _pattern_ptr = nullptr
             ) :
        color{_color},
        ambient{_ambient},
        diffuse{_diffuse},
        specular{_specular},
        shininess{_shininess},
        reflective{_reflective},
        transparency{_transparency},
        refractive_index{_refractive_index},
        pattern_ptr{_pattern_ptr} {}

    Color color{1, 1, 1};
    float ambient{0.1};
    float diffuse{0.9};
    float specular{0.9};
    float shininess{200.0};
    float reflective{0.0};
    float transparency{0.0};
    float refractive_index{1.0};

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
