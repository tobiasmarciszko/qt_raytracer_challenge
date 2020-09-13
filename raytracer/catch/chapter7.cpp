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
#include "camera.h"
#include "worlds.h"
#include "engine.h"
#include "equal.h"

using namespace Worlds;
using namespace Raytracer::Engine;

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
    REQUIRE(xs.at(0).t == 4);
    REQUIRE(xs.at(1).t == 4.5);
    REQUIRE(xs.at(2).t == 5.5);
    REQUIRE(xs.at(3).t == 6);
}

TEST_CASE("Precomputing the state of an intersection") {
    const auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
    std::shared_ptr<Shape> shape = std::make_shared<Sphere>(Sphere());
    const auto i = Intersection(4, shape.get());
    const auto comps = prepare_computations(i, r);

    REQUIRE(equal(comps.t, i.t));
    REQUIRE(comps.object == i.object);
    REQUIRE(comps.point == Point(0, 0, -1));
    REQUIRE(comps.eyev == Vector(0, 0, -1));
    REQUIRE(comps.normalv == Vector(0, 0, -1));
}

TEST_CASE("The hit, when an intersection occurs on the outside") {
    const auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
    std::shared_ptr<Shape> shape = std::make_shared<Sphere>(Sphere());
    const auto i = Intersection(4, shape.get());
    const auto comps = prepare_computations(i, r);

    REQUIRE(comps.inside == false);
}

TEST_CASE("The hit, when an intersection occurs on the inside") {
    const auto r = Ray(Point(0, 0, 0), Vector(0, 0, 1));
    std::shared_ptr<Shape> shape = std::make_shared<Sphere>(Sphere());
    const auto i = Intersection(1, shape.get());
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

    const auto shape = w.shapes.at(0).get();
    const auto i = Intersection(4, shape);
    const auto comps = prepare_computations(i, r);
    const auto c = shade_hit(w, comps);

    REQUIRE(c == Color(0.38066, 0.47583, 0.2855));
}

TEST_CASE("Shading an intersection from the inside") {
    auto w = default_world();
    w.lights.clear();
    w.lights.emplace_back(std::make_unique<PointLight>(PointLight(Point(0, 0.25, 0), Color(1, 1, 1))));

    const auto r = Ray(Point(0, 0, 0), Vector(0, 0, 1));

    const auto shape = w.shapes.at(1).get();
    const auto i = Intersection(0.5, shape);
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
    auto outer = w.shapes.at(0).get();
    auto m = outer->material;
    m.ambient = 1;
    outer->set_material(m);

    auto inner = w.shapes.at(1).get();
    auto m2 = inner->material;
    m2.ambient = 1;
    inner->set_material(m2);

    const auto r = Ray(Point(0, 0, 0.75), Vector(0, 0, -1));
    const auto c = color_at(w, r);

    REQUIRE(c == inner->material.color);
}

TEST_CASE("The transformation matrix for the default orientation") {
    const auto from = Point(0, 0, 0);
    const auto to = Point(0, 0, -1);
    const auto up = Vector(0, 1, 0);

    const auto t = view_transform(from, to, up);

    REQUIRE(t == identity_matrix);
}

TEST_CASE("A view transformation matrix looking in positive z direction") {
    const auto from = Point(0, 0, 0);
    const auto to = Point(0, 0, 1);
    const auto up = Vector(0, 1, 0);

    const auto t = view_transform(from, to, up);

    REQUIRE(t == scaling(-1, 1, -1));
}

TEST_CASE("The view transformation moves the world") {
    const auto from = Point(0, 0, 8);
    const auto to = Point(0, 0, 0);
    const auto up = Vector(0, 1, 0);

    const auto t = view_transform(from, to, up);

    REQUIRE(t == translation(0, 0, -8));
}

TEST_CASE("An arbitrary view transformation") {
    const auto from = Point(1, 3, 2);
    const auto to = Point(4, -2, 8);
    const auto up = Vector(1, 1, 0);

    const auto t = view_transform(from, to, up);

    const auto expected = Matrix<4,4>{
         -0.50709, 0.50709,  0.67612, -2.36643,
          0.76772, 0.60609,  0.12122, -2.82843,
         -0.35857, 0.59761, -0.71714,  0.00000,
          0.00000, 0.00000,  0.00000,  1.00000
    };

    REQUIRE(t == expected);
}

TEST_CASE("Constructing a camera") {

    const auto hsize = 160;
    const auto vsize = 120;
    const auto field_of_view = M_PI_2;

    const auto c = Camera(hsize, vsize, field_of_view);

    REQUIRE(c.hsize == 160);
    REQUIRE(c.vsize == 120);
    REQUIRE(equal(c.field_of_view, M_PI_2));
    REQUIRE(c.transform() == identity_matrix);
}

TEST_CASE("The pixel size for a horizontal canvas") {

    const auto c = Camera(200, 125, M_PI_2);

    REQUIRE(equal(c.pixel_size, 0.01));
}

TEST_CASE("The pixel size for a vertical canvas") {

    const auto c = Camera(125, 200, M_PI_2);

    REQUIRE(equal(c.pixel_size, 0.01));
}

TEST_CASE("Constructing a ray through the center of the canvas") {

    const auto c = Camera(201, 101, M_PI_2);
    const auto r = ray_for_pixel(c, 100, 50);

    REQUIRE(r.origin() == Point(0, 0, 0));
    REQUIRE(r.direction() == Vector(0, 0, -1));
}

TEST_CASE("Constructing a ray through a corner of the canvas") {

    const auto c = Camera(201, 101, M_PI_2);
    const auto r = ray_for_pixel(c, 0, 0);

    REQUIRE(r.origin() == Point(0, 0, 0));
    REQUIRE(r.direction() == Vector(0.66519, 0.33259, -0.66851));
}

TEST_CASE("Constructing a ray when the camera is transformed") {

    auto c = Camera(201, 101, M_PI_2);
    c.set_transform(rotation_y(M_PI_4) * translation(0, -2, 5));
    const auto r = ray_for_pixel(c, 100, 50);

    REQUIRE(r.origin() == Point(0, 2, -5));
    REQUIRE(r.direction() == Vector(M_SQRT2/2, 0, -M_SQRT2/2));
}

