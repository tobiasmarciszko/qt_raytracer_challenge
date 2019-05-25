#ifndef LIGHTING_H
#define LIGHTING_H

#include <cmath>
#include "color.h"
#include "material.h"
#include "light.h"
#include "vector.h"
#include "point.h"

inline Color lighting(
    const Material& material,
    const Light& light,
    const Point& point,
    const Vector& eyev,
    const Vector& normalv) {

    const auto black = Color(0, 0, 0);
    Color ambient;
    Color diffuse;
    Color specular;

    // combine the surface color with the light's color/intensity
    const Color effective_color = material.color * light.intensity();

    // find the direction to the light source
    const Vector lightv = Vector(light.position() - point).normalize();

    // compute the ambient contribution
    ambient = effective_color * material.ambient;

    // light_dot_normal represents the cosine of the angle between the
    // light vector and the normal vector. A negative number means the
    // light is on the other side of the surface.
    const auto light_dot_normal = lightv.dot(normalv);

    if (light_dot_normal < 0) {
        diffuse = black;
        specular = black;
    } else {
        // compute the diffuse contribution
        diffuse = effective_color * material.diffuse * light_dot_normal;

        // reflect_dot_eye represents the cosine of the angle between the
        // reflection vector and the eye vector. A negative number means the
        // light reflects away from the eye
        const Vector reflectv = Vector(-lightv).reflect(normalv);
        const double reflect_dot_eye = reflectv.dot(eyev);

        if (reflect_dot_eye <= 0) {
            specular = black;
        } else {
            // compute the specular contribution
            const auto factor = std::pow(reflect_dot_eye, material.shininess);
            specular = light.intensity() * material.specular * factor;
        }
    }

    // Add the three contributions together to get the final shading
    const auto result = ambient + diffuse + specular;
    return result;
}

#endif //LIGHTING_H
