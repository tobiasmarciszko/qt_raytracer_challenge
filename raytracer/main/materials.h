#ifndef MATERIALS_H
#define MATERIALS_H

#include "material.h"

namespace Materials {
    const Material glass
    {
        black,  // color
        0,      // ambient
        0,      // diffuse
        1,      // specular
        300,    // shininess
        0.9,    // reflective
        1,      // transparency
        1.5     // refractive index
    };

    const Material diamond
        {
            black,  // color
            0,      // ambient
            0,      // diffuse
            1,      // specular
            300,    // shininess
            0.9,    // reflective
            1,      // transparency
            2.417   // refractive index
        };
}

#endif // MATERIALS_H
