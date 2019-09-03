#ifndef LIGHTING_H
#define LIGHTING_H

#include <cmath>
#include <memory>
#include "color.h"
#include "material.h"
#include "light.h"
#include "vector.h"
#include "point.h"
#include "pattern.h"
#include "shape.h"

enum class LightingModel {
    Phong,
    BlinnPhong
};

inline Color lighting(
    const Material& material,
    const std::shared_ptr<const Shape> object,
    const Light& light,
    const Point& point,
    const Vector& eyev,
    const Vector& normalv,
    const bool in_shadow = false,
    const LightingModel& lightingModel = LightingModel::Phong
    ) {

    const auto black = Color(0, 0, 0);
    Color ambient;
    Color diffuse;
    Color specular;

    Color materialColor;

    // Check if the material has a pattern first
    if (material.pattern.has_value()) {
        materialColor = stripe_at_object(material.pattern.value(), object, point);
    } else {
        materialColor = material.color;
    }

    // combine the surface color with the light's color/intensity
    const Color effective_color = materialColor * light.intensity();

    // find the direction to the light source
    const Vector lightv = Vector(light.position() - point).normalize();

    // compute the ambient contribution
    ambient = effective_color * material.ambient;

    // light_dot_normal represents the cosine of the angle between the
    // light vector and the normal vector. A negative number means the
    // light is on the other side of the surface.
    const auto light_dot_normal = lightv.dot(normalv);

    if (light_dot_normal < 0 || in_shadow) {
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

            double factor;

            if (lightingModel == LightingModel::Phong) {
                factor = std::pow(reflect_dot_eye, material.shininess);
            }

            if (lightingModel == LightingModel::BlinnPhong) {
                const auto halfway = Vector(lightv + eyev).normalize();
                const auto halfway_dot = normalv.dot(halfway);

                factor = std::pow(halfway_dot, material.shininess);
            }

            specular = light.intensity() * material.specular * factor;
        }
    }

    // Add the three contributions together to get the final shading
    return ambient + diffuse + specular;
}

#endif //LIGHTING_H
