#include "catch.hpp"
#include "material.h"
#include "vector.h"
#include "lighting.h"
#include "sphere.h"
#include "shape.h"
#include "plane.h"
#include "math.h"
#include "computations.h"
#include "world.h"

TEST_CASE("Reflectivity for the default material")
{
    Material m;
    REQUIRE(equal(m.reflective, 0.0f));
}

TEST_CASE("Precomputing the reflection vector")
{
    const std::shared_ptr<Shape> shape = std::make_shared<Plane>(Plane());
    Ray r(Point(0, 1, -1), Vector(0, -M_SQRT2/2, M_SQRT2/2));
    Intersection i(M_SQRT2, shape.get());
    Computations comps = prepare_computations(i, r);
    REQUIRE(comps.reflectv == Vector(0, M_SQRT2/2, M_SQRT2/2));
}

TEST_CASE("The reflected color for a nonreflective material")
{
    World w = default_world();
    Ray r(Point(0, 0, 0), Vector(0, 0, 1));
    auto shape = w.shapes.at(1).get();
    Material m = shape->material();
    m.ambient = 1;
    shape->set_material(m);

    Intersection i(1, shape);

    Computations comps = prepare_computations(i, r);
    Color color = reflected_color(w, comps);
    REQUIRE(color == Color(0, 0, 0));
}

TEST_CASE("The reflected color for a reflective material")
{
    World w = default_world();

    std::shared_ptr<Shape> shape = std::make_shared<Plane>(Plane());
    Material m = shape->material();
    m.reflective = 0.5;
    shape->set_material(m);
    shape->set_transform(translation(0, -1, 0));
    w.shapes.emplace_back(shape);

    Ray r(Point(0, 0, -3), Vector(0, -M_SQRT2/2, M_SQRT2/2));
    Intersection i(M_SQRT2, shape.get());

    Computations comps = prepare_computations(i, r);
    Color color = reflected_color(w, comps);
    REQUIRE(color == Color(0.19032, 0.2379, 0.14274));
}


TEST_CASE("shade_hit() with a reflective material")
{
    World w = default_world();

    std::shared_ptr<Shape> shape = std::make_shared<Plane>(Plane());
    Material m = shape->material();
    m.reflective = 0.5;
    shape->set_material(m);
    shape->set_transform(translation(0, -1, 0));
    w.shapes.emplace_back(shape);

    Ray r(Point(0, 0, -3), Vector(0, -M_SQRT2/2, M_SQRT2/2));
    Intersection i(M_SQRT2, shape.get());

    Computations comps = prepare_computations(i, r);
    Color color = shade_hit(w, comps);
    REQUIRE(color == Color(0.87677, 0.92436, 0.82918));
}

TEST_CASE("color_at() with mutually reflective surfaces")
{
    // This test should not cause an infinite recursion!

    World w;
    Light light = PointLight(Point(0,0,0), Color(1,1,1));
    w.lights.emplace_back(light);

    std::shared_ptr<Shape> lower = std::make_shared<Plane>(Plane());
    Material m = lower->material();
    m.reflective = 1;
    lower->set_material(m);
    lower->set_transform(translation(0, -1, 0));
    w.shapes.emplace_back(lower);

    std::shared_ptr<Shape> upper = std::make_shared<Plane>(Plane());
    m = lower->material();
    m.reflective = 1;
    upper->set_material(m);
    upper->set_transform(translation(0, 11, 0));
    w.shapes.emplace_back(upper);

    Ray r(Point(0, 0, 0), Vector(0, 1, 0));

    Color color = color_at(w, r);
    REQUIRE(true);
}

TEST_CASE("The reflected color at the maximum recursive depth")
{
    World w = default_world();

    std::shared_ptr<Shape> shape = std::make_shared<Plane>(Plane());
    Material m = shape->material();
    m.reflective = 0.5;
    shape->set_material(m);
    shape->set_transform(translation(0, -1, 0));
    w.shapes.emplace_back(shape);

    Ray r(Point(0, 0, -3), Vector(0, -M_SQRT2/2, M_SQRT2/2));
    Intersection i(M_SQRT2, shape.get());

    Computations comps = prepare_computations(i, r);
    Color color = reflected_color(w, comps, 0);
    REQUIRE(color == Color(0, 0, 0));
}
