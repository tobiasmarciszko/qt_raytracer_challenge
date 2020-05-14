#include "catch.hpp"

#include "point.h"
#include "vector.h"
#include "color.h"
#include "light.h"
#include "material.h"
#include "world.h"
#include "sphere.h"
#include "computations.h"
#include "worlds.h"
#include "engine.h"
#include "equal.h"

using namespace Worlds;
using namespace Raytracer::Engine;

TEST_CASE("Lighting with the surface in shadow")
{
    const auto m = Material();
    const auto position = Point(0, 0, 0);

    const auto eyev = Vector(0, 0, -1);
    const auto normalv = Vector(0, 0, -1);
    const auto light = PointLight(Point(0, 0, -10), Color(1, 1, 1));
    const auto intensity = 0.0;

    const auto object = std::make_shared<Sphere>().get();

    const auto result = lighting(m, object, light, position, eyev, normalv, intensity);

    REQUIRE(result == Color(0.1, 0.1, 0.1));
}

TEST_CASE("There is no shadow when nothing is collinear with point and light")
{
    const auto w = default_world();
    const auto p = Point(0, 10, 0);

    REQUIRE_FALSE(is_shadowed(w, w.lights.front().position, p));
}

TEST_CASE("The shadow when an object is between the point and the light")
{
    const auto w = default_world();
    const auto p = Point(10, -10, 10);

    REQUIRE(is_shadowed(w, w.lights.front().position, p));
}

TEST_CASE("There is no shadow when an object is behind the light")
{
    const auto w = default_world();
    const auto p = Point(-20, 20, -20);

    REQUIRE_FALSE(is_shadowed(w, w.lights.front().position, p));
}

TEST_CASE("There is no shadow when an object is behind the point")
{
    const auto w = default_world();
    const auto p = Point(-2, 2, -2);

    REQUIRE_FALSE(is_shadowed(w, w.lights.front().position,   p));
}

TEST_CASE("shade_hit() is given an intersection in shadow")
{
    auto w = World();

    w.lights.emplace_back(PointLight(Point(0, 0, -10), Color(1, 1, 1)));

    Sphere s1;
    w.shapes.emplace_back(std::make_unique<Sphere>(s1));

    Sphere s2;
    s2.set_transform(translation(0, 0, 10));
    w.shapes.emplace_back(std::make_unique<Sphere>(s2));

    const auto r = Ray(Point(0, 0, 5), Vector(0, 0, 1));
    const auto i = Intersection(4, w.shapes.at(1).get());

    const auto comps = prepare_computations(i, r);
    const auto c = shade_hit(w, comps);

    REQUIRE(c == Color(0.1, 0.1, 0.1));
}

TEST_CASE("The hit should offset the point")
{
    const auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
    std::shared_ptr<Shape> shape = std::make_shared<Sphere>(Sphere());
    shape->set_transform(translation(0, 0, 1));
    auto i = Intersection(5, shape.get());
    auto comps = prepare_computations(i, r);

    REQUIRE(comps.over_point.z < - EPSILON/2);
    REQUIRE(comps.point.z > comps.over_point.z);
}
