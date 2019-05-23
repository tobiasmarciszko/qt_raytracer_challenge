#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>
#include "color.h"

struct Material {
    Color color = Color(1, 1, 1);
    double ambient   = 0.1;
    double diffuse   = 0.9;
    double specular  = 0.9;
    double shininess = 200.0;

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
