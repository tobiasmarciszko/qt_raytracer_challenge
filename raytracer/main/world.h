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

    for (auto& object: w.shapes) {
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
            comps.object->material(),
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


inline Matrix<4,4> view_transform(const Point& from, const Point& to, const Vector& up) {

    const auto forward = Vector(to - from).normalize();
    const auto upn = up.normalize();
    const auto left = forward.cross(upn);
    const auto true_up = left.cross(forward);

    const auto orientation = Matrix<4,4>{
        left.x(),      left.y(),     left.z(),    0,
        true_up.x(),   true_up.y(),  true_up.z(), 0,
        -forward.x(), -forward.y(), -forward.z(), 0,
        0,             0,            0,           1
    };

    return orientation * translation(-from.x(), -from.y(), -from.z());
}

#endif //WORLD_H
