#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include "light.h"
#include "sphere.h"
#include "material.h"
#include "intersection.h"
#include "computations.h"
#include "lighting.h"

struct World
{
    std::vector<Light> lights{};
    std::vector<Sphere> objects{};
};

World default_world()
{

    World world;

    // Default world:
    const auto light = PointLight(Point(-10, 10, -10), Color(1, 1, 1));
    world.lights = {light};

    auto sphere1 = Sphere();
    auto material = Material();
    material.color = Color(0.8, 1.0, 0.6);
    material.diffuse = 0.7;
    material.specular = 0.2;
    sphere1.set_material(material);

    auto sphere2 = Sphere();
    auto transform = scaling(0.5, 0.5, 0.5);
    sphere2.set_transform(transform);

    world.objects = {sphere1, sphere2};

    return world;
}

inline Intersections intersect_world(const World& w, const Ray& r)
{
    Intersections is;

    for (const auto &object: w.objects) {
        const Intersections xs = intersect(r, object);
        is.insert(std::end(is), std::begin(xs), std::end(xs));
    }

    std::sort(is.begin(), is.end(), [](
        const Intersection &i1,
        const Intersection &i2) -> bool
    {
        return i1.t() < i2.t();
    });

    return is;
}

inline Color shade_hit(const World& w, const Computations& comps) {

    Color c = Color(0, 0, 0);
    for (const auto& light: w.lights) {
        const auto color = lighting(
            comps.object.material(),
            light,
            comps.point,
            comps.eyev,
            comps.normalv);

        c = c + color;
    }

    return c;
}

inline Color color_at(const World& w, const Ray& r) {

    const auto black = Color(0, 0, 0);

    const auto is = intersect_world(w, r);
    const auto h = hit(is);
    if (!h.has_value()) {
        return black;
    }

    const auto comps = prepare_computations(h.value(), r);
    const auto color = shade_hit(w, comps);

    return color;
}

#endif //WORLD_H
