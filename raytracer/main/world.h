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
    auto material = Material();
    material.color = Color(0.8, 1.0, 0.6);
    material.diffuse = 0.7;
    material.specular = 0.2;
    sphere1->set_material(material);

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

Color shade_hit(const World& w, const Computations& comps, const LightingModel& lightingModel = LightingModel::Phong, int remaining = 4);

inline Color color_at(const World& w, const Ray& r, const LightingModel& lightingModel = LightingModel::Phong, int remaining = 4) {

    const Color black{0, 0, 0};

    const auto is = intersect_world(w, r);
    const auto h = hit(is);
    if (!h.has_value()) {
        return black;
    }

    const auto comps = prepare_computations(h.value(), r);
    const auto color = shade_hit(w, comps, lightingModel, remaining);

    return color;
}

inline Color reflected_color(const World& w, const Computations& comps, const LightingModel& lightingModel = LightingModel::Phong, int remaining = 4) {

    // Prevent infinite recursion
    if (remaining <= 0) {
        return Color(0, 0, 0);
    }

    if (equal(comps.object->material().reflective, 0)) {
        return Color(0, 0, 0);
    }

    const Ray reflect_ray(comps.over_point, comps.reflectv);
    const Color color = color_at(w, reflect_ray, lightingModel, remaining - 1);

    return color * comps.object->material().reflective;
}

inline Color reflected_color(const World& w, const Computations& comps, int remaining) {
    return reflected_color(w, comps, LightingModel::Phong, remaining);
}

inline Color shade_hit(const World& w, const Computations& comps, const LightingModel& lightingModel, int remaining) {

    Color c{0, 0, 0};
    for (const auto& light: w.lights) {
        const auto in_shadow = is_shadowed(w, comps.over_point, light);

        const auto surface = lighting(
            comps.object->material(),
            comps.object,
            light,
            comps.over_point,
            comps.eyev,
            comps.normalv,
            in_shadow,
            lightingModel);

        const auto reflected = reflected_color(w, comps, lightingModel, remaining);

        c = c + (surface + reflected);
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

inline Color refracted_color(const World& world, const Computations& comps, int remaining) {

    // Prevent infinite recursion
    if (remaining <= 0) {
        return black;
    }

    if (equal(comps.object->material().transparency, 0)) {
        return black;
    }

    return white;
}

#endif //WORLD_H
