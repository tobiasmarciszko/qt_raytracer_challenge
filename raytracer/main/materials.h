#ifndef MATERIALS_H
#define MATERIALS_H

#include "material.h"

namespace Materials {
    struct Glass : public Material {
        Glass() {
            color = black;
            ambient = 0;
            diffuse = 0;
            transparency = 1.0;
            refractive_index = 1.5;
            reflective = 0.5;
            specular = 1;
            shininess = 300;
        }
    };
}

#endif // MATERIALS_H
