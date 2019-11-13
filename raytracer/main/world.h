#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <typeinfo>
#include "light.h"
#include "sphere.h"
#include "material.h"
#include "intersection.h"
#include "computations.h"
#include "lighting.h"

struct World
{
    std::vector<Light> lights;
    std::vector<std::shared_ptr<Shape>> shapes;
};

inline World default_world()
{
    World world;

    // Default world:
    const auto light = PointLight(Point(-10, 10, -10), Color(1, 1, 1));
    world.lights = {light};

    auto sphere1 = std::make_shared<Sphere>(Sphere());
    Material material;
    material.color = Color(0.8, 1.0, 0.6);
    material.diffuse = 0.7;
    material.specular = 0.2;
    sphere1->material = material;

    auto sphere2 = std::make_shared<Sphere>(Sphere());
    auto transform = scaling(0.5, 0.5, 0.5);
    sphere2->set_transform(transform);

    world.shapes = {sphere1, sphere2};

    return world;
}

inline Intersections intersect_world(const World& w, const Ray& r)
{
    Intersections is;

    for (const auto& object: w.shapes) {
        const Intersections xs = object->intersect(r);

        for (const auto& intersection: xs) {
            is.emplace_back(intersection);
        }
    }

    std::sort(is.begin(), is.end(), [](
        const Intersection &i1,
        const Intersection &i2) -> bool
    {
        return i1.t < i2.t;
    });

    return is;
}

inline bool is_shadowed(const World& world, const Point& point, const Light& light) {

        const Vector v = light.position() - point;
        const auto distance = v.magnitude();
        const auto direction = v.normalize();

        const Ray r = Ray(point, direction);

        const auto intersections = intersect_world(world, r);
        const auto h = hit(intersections);

        if (h.has_value() && (h->t < distance)) {
            return true;
        } else {
            return false;
        }
}

inline bool is_shadowed(const World& world, const Point& point) {
    return is_shadowed(world, point, world.lights.front());
}

Color shade_hit(const World& w, const Computations& comps, const LightingModel& lightingModel = LightingModel::Phong, int remaining = 5);
Color refracted_color(const World& world, const Computations& comps, const LightingModel& lightingModel = LightingModel::Phong, int remaining = 4);
float schlick(const Computations& comps);

inline Color color_at(const World& w, const Ray& r, const LightingModel& lightingModel = LightingModel::Phong, int remaining = 4) {

    const Color black{0, 0, 0};

    const auto is = intersect_world(w, r);
    const auto h = hit(is);
    if (!h.has_value()) {
        return Color(0, 0, 0);
    }

    const auto comps = prepare_computations(h.value(), r, is);
    const auto color = shade_hit(w, comps, lightingModel, remaining);

    return color;
}

inline Color reflected_color(const World& w, const Computations& comps, const LightingModel& lightingModel = LightingModel::Phong, int remaining = 4) {

    // Prevent infinite recursion
    if (remaining <= 0) {
        return black;
    }

    if (equal(comps.object->material.reflective, 0)) {
        return black;
    }

    const Ray reflect_ray(comps.over_point, comps.reflectv);
    const Color color = color_at(w, reflect_ray, lightingModel, remaining - 1);

    return color * comps.object->material.reflective;
}

inline Color reflected_color(const World& w, const Computations& comps, int remaining) {
    return reflected_color(w, comps, LightingModel::Phong, remaining);
}

inline Color shade_hit(const World& w, const Computations& comps, const LightingModel& lightingModel, int remaining) {

    Color c{0, 0, 0};
    for (const auto& light: w.lights) {
        const auto in_shadow = is_shadowed(w, comps.over_point, light);

        const auto surface = lighting(
            comps.object->material,
            comps.object,
            light,
            comps.over_point,
            comps.eyev,
            comps.normalv,
            in_shadow,
            lightingModel);

        const auto reflected = reflected_color(w, comps, lightingModel, remaining);
        const auto refracted = refracted_color(w, comps, lightingModel, remaining);


        // If the object is both reflective and refractive, the schlick approximation is used:

        const auto material = comps.object->material;
        if (material.reflective > 0 && material.transparency >0) {
            const auto reflectance = schlick(comps);
            c = c + (surface + reflected * reflectance + refracted * (1 - reflectance));
        } else
        {
            c = c + (surface + reflected + refracted);
        }
    }

    return c;
}

inline Matrix<4,4> view_transform(const Point& from, const Point& to, const Vector& up) {

    const auto forward = Vector(to - from).normalize();
    const auto upn = up.normalize();
    const auto left = forward.cross(upn);
    const auto true_up = left.cross(forward);

    const auto orientation = Matrix<4,4>{
        left.x,      left.y,     left.z,    0,
        true_up.x,   true_up.y,  true_up.z, 0,
        -forward.x, -forward.y, -forward.z, 0,
        0,             0,            0,           1
    };

    return orientation * translation(-from.x, -from.y, -from.z);
}

inline Color refracted_color(const World& world, const Computations& comps, const LightingModel& lightingModel, int remaining) {

    // Prevent infinite recursion
    if (remaining <= 0) {
        return black;
    }

    if (equal(comps.object->material.transparency, 0)) {
        return black;
    }

    // Check for total internal reflection:
    // # Find the ratio of first index of refraction to the second.
    // # (Yup, this is inverted from the definition of Snell's Law.)
    const auto n_ratio = comps.n1 / comps.n2;

    // # cos(theta_i) is the same as the dot product of the two vectors
    const auto cos_i = comps.eyev.dot(comps.normalv);

    // # Find sin(theta_t)^2 via trigonometric identity
    const auto sin2_t = (n_ratio * n_ratio) * (1 - (cos_i * cos_i));

    // Total internal reflection if the value is greater than one!
    if (sin2_t > 1) return black;

    // Refracted Color with a refracted ray

    // # Find cos(theta_t) via trigonometric identity
    const auto cos_t = std::sqrt(1.0 - sin2_t);

    // # Compute the direction of the refracted ray
    const auto direction = comps.normalv * (n_ratio * cos_i - cos_t) - comps.eyev * n_ratio;

    // # Create the refracted ray
    const Ray refract_ray{comps.under_point, direction};

    // # Find the color of the refracted ray, making sure to multiply
    // # by the transparency value to account for any opacity
    return color_at(world, refract_ray, lightingModel, remaining - 1) * comps.object->material.transparency;
}

inline float schlick(const Computations& comps) {
    // # find the cosine of the angle between the eye and normal vectors
    auto cos = comps.eyev.dot(comps.normalv);

    // # total internal reflection can only occur if n1 > n2
    if (comps.n1 > comps.n2) {
        const auto n = comps.n1 / comps.n2;
        const auto sin2_t = (n * n) * (1 - (cos * cos));

        if (sin2_t > 1) return 1;

        // # compute cosine of theta_t using trig identity
        const auto cos_t = std::sqrt(1 - sin2_t);

        // when n1 > n2, use cos(theta_t) instead
        cos = cos_t;
    }

    const auto r0 = std::pow((comps.n1 - comps.n2) / (comps.n1 + comps.n2), 2);
    return r0 + (1 - r0) * std::pow((1 - cos), 5);
}

#endif //WORLD_H
