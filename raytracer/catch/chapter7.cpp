#include "catch.hpp"
#include "tuple.h"
#include "point.h"
#include "tuple.h"
#include "point.h"
#include "vector.h"
#include "color.h"
#include "canvas.h"
#include "matrix.h"
#include "ray.h"
#include "sphere.h"
#include "light.h"
#include "material.h"
#include "world.h"
#include "computations.h"

TEST_CASE("Creating a world") {
    const auto w = World();

    REQUIRE(w.lights.empty());
    REQUIRE(w.shapes.empty());
}

TEST_CASE("Default world") {

    const auto w = default_world();

    REQUIRE(w.lights.size() == 1);
    REQUIRE(w.shapes.size() == 2);
}

TEST_CASE("Intersect a world with a ray") {

    const auto w = default_world();
    const auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));

    const auto xs = intersect_world(w, r);

    REQUIRE(xs.size() == 4);
    REQUIRE(xs.at(0).t() == 4);
    REQUIRE(xs.at(1).t() == 4.5);
    REQUIRE(xs.at(2).t() == 5.5);
    REQUIRE(xs.at(3).t() == 6);
}

TEST_CASE("Precomputing the state of an intersection") {
    const auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
    const auto shape = std::make_shared<Sphere>(Sphere());
    const auto i = intersection(4, shape);
    const auto comps = prepare_computations(i, r);

    REQUIRE(equal(comps.t, i.t()));
    REQUIRE(comps.object == i.object());
    REQUIRE(comps.point == Point(0, 0, -1));
    REQUIRE(comps.eyev == Vector(0, 0, -1));
    REQUIRE(comps.normalv == Vector(0, 0, -1));
}

TEST_CASE("The hit, when an intersection occurs on the outside") {
    const auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
    const auto shape = std::make_shared<Sphere>(Sphere());
    const auto i = intersection(4, shape);
    const auto comps = prepare_computations(i, r);

    REQUIRE(comps.inside == false);
}

TEST_CASE("The hit, when an intersection occurs on the inside") {
    const auto r = Ray(Point(0, 0, 0), Vector(0, 0, 1));
    const auto shape = std::make_shared<Sphere>(Sphere());
    const auto i = intersection(1, shape);
    const auto comps = prepare_computations(i, r);

    REQUIRE(comps.point == Point(0, 0, 1));
    REQUIRE(comps.eyev == Vector(0, 0, -1));

    REQUIRE(comps.inside == true);
    // should have been 0,0,1 but is inverted
    REQUIRE(comps.normalv == Vector(0, 0, -1));
}

TEST_CASE("Shading an intersection") {
    const auto w = default_world();
    const auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));

    const auto shape = w.shapes.at(0);
    const auto i = intersection(4, shape);
    const auto comps = prepare_computations(i, r);
    const auto c = shade_hit(w, comps);

    REQUIRE(c == Color(0.38066, 0.47583, 0.2855));
}

TEST_CASE("Shading an intersection from the inside") {
    auto w = default_world();
    w.lights = {PointLight(Point(0, 0.25, 0), Color(1, 1, 1))};
    const auto r = Ray(Point(0, 0, 0), Vector(0, 0, 1));

    const auto shape = w.shapes.at(1);
    const auto i = intersection(0.5, shape);
    const auto comps = prepare_computations(i, r);
    const auto c = shade_hit(w, comps);

    REQUIRE(c == Color(0.90498, 0.90498, 0.90498));
}

TEST_CASE("The color when a ray misses") {
    const auto w = default_world();
    const auto r = Ray(Point(0, 0, -5), Vector(0, 1, 0));
    const auto c = color_at(w, r);

    REQUIRE(c == Color(0, 0, 0));
}

TEST_CASE("The color when a ray hits") {
    const auto w = default_world();
    const auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
    const auto c = color_at(w, r);

    REQUIRE(c == Color(0.38066, 0.47583, 0.2855));
}


TEST_CASE("The color with an intersection behind the ray") {
    auto w = default_world();
    auto outer = w.shapes.at(0);
    auto m = outer->material();
    m.ambient = 1;
    outer->set_material(m);

    auto inner = w.shapes.at(1);
    auto m2 = inner->material();
    m2.ambient = 1;
    inner->set_material(m2);

    const auto r = Ray(Point(0, 0, 0.75), Vector(0, 0, -1));
    const auto c = color_at(w, r);

    REQUIRE(c == inner->material().color);
}
