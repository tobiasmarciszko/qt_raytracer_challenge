#include "catch.hpp"
#include "material.h"
#include "vector.h"
#include "sphere.h"
#include "shape.h"
#include "plane.h"
#include "math.h"
#include "computations.h"
#include "world.h"
#include "worlds.h"
#include "engine.h"
#include "equal.h"

using namespace Worlds;
using namespace Raytracer::Engine;

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
    Material m = shape->material;
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
    Material m = shape->material;
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
    Material m = shape->material;
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
    Material m = lower->material;
    m.reflective = 1;
    lower->set_material(m);
    lower->set_transform(translation(0, -1, 0));
    w.shapes.emplace_back(lower);

    std::shared_ptr<Shape> upper = std::make_shared<Plane>(Plane());
    m = lower->material;
    m.reflective = 1;
    upper->set_material(m);
    upper->set_transform(translation(0, 11, 0));
    w.shapes.emplace_back(upper);

    Ray r(Point(0, 0, 0), Vector(0, 1, 0));

    color_at(w, r);

    REQUIRE(true);
}

TEST_CASE("The reflected color at the maximum recursive depth")
{
    World w = default_world();

    std::shared_ptr<Shape> shape = std::make_shared<Plane>(Plane());
    Material m = shape->material;
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

TEST_CASE("Transparency and Refractive Index for the default material")
{
    Material m;

    REQUIRE(equal(m.transparency, 0.0));
    REQUIRE(equal(m.refractive_index, 1.0));
}

TEST_CASE("A helper for producing a sphere with a glassy material")
{
    Sphere s = glass_sphere();

    REQUIRE(s.transform() == identity_matrix);
    REQUIRE(equal(s.material.transparency, 1.0));
    REQUIRE(equal(s.material.refractive_index, 1.5));
}


TEST_CASE("Finding n1 and n2 at various intersections")
{
    Material m;

    Sphere A{glass_sphere()};
    Sphere B{glass_sphere()};
    Sphere C{glass_sphere()};

    A.set_transform(scaling(2, 2, 2));
    m = A.material;
    m.refractive_index = 1.5;
    A.set_material(m);

    B.set_transform(translation(0, 0, -0.25));
    m = B.material;
    m.refractive_index = 2.0;
    B.set_material(m);

    C.set_transform(translation(0, 0, 0.25));
    m = C.material;
    m.refractive_index = 2.5;
    C.set_material(m);

    const Ray r{Point(0,0,-4), Vector(0,0,1)};
    const Intersections xs{{2, &A}, {2.75, &B}, {3.25, &C}, {4.75, &B}, {5.25, &C}, {6, &A}};

    const std::vector<std::tuple<uint, float, float>> data{
        {0, 1.0, 1.5},
        {1, 1.5, 2.0},
        {2, 2.0, 2.5},
        {3, 2.5, 2.5},
        {4, 2.5, 1.5},
        {5, 1.5, 1.0},
    };

    for (const auto& [index, n1, n2]: data) {
        const Computations comps = prepare_computations(xs.at(index), r, xs);
        REQUIRE(equal(comps.n1, n1));
        REQUIRE(equal(comps.n2, n2));
    }
}


TEST_CASE("The under point is offset below the surface") {
    const Ray r{Point{0,0,-5}, Vector{0,0,1}};

    std::shared_ptr<Shape> shape = std::make_shared<Sphere>(glass_sphere());
    shape->set_transform(translation(0,0,1));
    Intersection i{5, shape.get()};
    Intersections xs{i};

    Computations comps = prepare_computations(i, r, xs);


    REQUIRE(comps.under_point.z > EPSILON /2);
    REQUIRE(comps.point.z < comps.under_point.z);
}

TEST_CASE("The refracted color with an opaque surface") {
    World w = default_world();
    auto shape = w.shapes.front().get();

    Ray r{Point{0,0,-5}, Vector{0,0,1}};
    Intersections xs{{4, shape}, {6, shape}};

    Computations comps = prepare_computations(xs.at(0), r, xs);
    Color c = refracted_color(w, comps, LightingModel::Phong, 5);

    REQUIRE(c == Color(0,0,0));
}

TEST_CASE("The refracted color at the maximum recursive depth")
{
    World w = default_world();

    auto shape = w.shapes.front().get();
    Material m = shape->material;
    m.transparency = 1.0;
    m.refractive_index = 1.5;
    shape->set_material(m);

    Ray r{Point{0, 0, -5}, Vector{0, 0, 1}};
    Intersections xs{{4, shape}, {6, shape}};

    Computations comps = prepare_computations(xs.at(0), r, xs);
    Color color = refracted_color(w, comps, LightingModel::Phong, 0);
    REQUIRE(color == Color(0, 0, 0));
}

TEST_CASE("The refracted color under total internal reflection")
{
    World w = default_world();

    auto shape = w.shapes.front().get();
    Material m = shape->material;
    m.transparency = 1.0;
    m.refractive_index = 1.5;
    shape->set_material(m);

    Ray r{Point{0, 0, M_SQRT2/2}, Vector{0, 1, 0}};
    Intersections xs{{-M_SQRT2/2, shape}, {M_SQRT2/2, shape}};

    // Inside the sphere, need to look at the second intersection!

    Computations comps = prepare_computations(xs.at(1), r, xs);
    Color color = refracted_color(w, comps, LightingModel::Phong, 5);
    REQUIRE(color == Color(0, 0, 0));
}

TEST_CASE("The refracted color with a refracted ray")
{
    World w = default_world();

    auto A = w.shapes.front().get();
    Material m1 = A->material;
    m1.ambient = 1.0;
    m1.pattern_ptr = test_pattern();
    A->set_material(m1);

    auto B = w.shapes.at(1).get();
    Material m2 = B->material;
    m2.transparency = 1.0;
    m2.refractive_index = 1.5;
    B->set_material(m2);

    Ray r{Point{0, 0, 0.1}, Vector{0, 1, 0}};
    Intersections xs{{-0.9899, A}, {-0.4899, B}, {0.4899, B}, {0.9899, A}};

    Computations comps = prepare_computations(xs.at(2), r, xs);
    Color color = refracted_color(w, comps, LightingModel::Phong, 5);
    REQUIRE(color == Color(0, 0.99888, 0.04725));
}


TEST_CASE("shade_hit() with a transparent material")
{
    World w = default_world();

    auto floor = std::make_shared<Plane>(Plane());
    floor->set_transform(translation(0, -1, 0));
    Material m1 = floor->material;
    m1.transparency = 0.5;
    m1.refractive_index = 1.5;
    floor->set_material(m1);
    w.shapes.emplace_back(floor);

    auto ball = std::make_shared<Sphere>(Sphere());
    ball->set_transform(translation(0, -3.5, -0.5));
    Material m2 = ball->material;
    m2.color = Color(1, 0, 0);
    m2.ambient = 0.5;
    ball->set_material(m2);
    w.shapes.emplace_back(ball);

    Ray r{Point{0, 0, -3}, Vector{0, -M_SQRT2/2, M_SQRT2/2}};
    Intersections xs{{M_SQRT2, floor.get()}};

    Computations comps = prepare_computations(xs[0], r, xs);
    Color color = shade_hit(w, comps, LightingModel::Phong, 5);
    REQUIRE(color == Color(0.93642, 0.68642, 0.68642));
}

TEST_CASE("The Schlick approximation under total internal reflection")
{
    auto shape = std::make_shared<Sphere>(glass_sphere());

    const Ray r{Point{0, 0, M_SQRT2/2}, Vector{0, 1, 0}};
    const Intersections xs{{-M_SQRT2/2, shape.get()}, {M_SQRT2/2, shape.get()}};

    const Computations comps = prepare_computations(xs[1], r, xs);

    const float reflectance = schlick(comps);
    REQUIRE(equal(reflectance, 1.0));
}

TEST_CASE("The Schlick approximation with a perpendicular viewing angle")
{
    auto shape = std::make_shared<Sphere>(glass_sphere());

    const Ray r{Point{0, 0, 0}, Vector{0, 1, 0}};
    const Intersections xs{{-1, shape.get()}, {-1, shape.get()}};

    const Computations comps = prepare_computations(xs[1], r, xs);

    const float reflectance = schlick(comps);
    REQUIRE(equal(reflectance, 0.04));
}


TEST_CASE("The Schlick approximation with small angle and n2 > n1")
{
    auto shape = std::make_shared<Sphere>(glass_sphere());

    const Ray r{Point{0, 0.99, -2}, Vector{0, 0, 1}};
    const Intersections xs{{1.8589, shape.get()}};

    const Computations comps = prepare_computations(xs[0], r, xs);

    const float reflectance = schlick(comps);
    REQUIRE(equal(reflectance, 0.48873));
}

TEST_CASE("shade_hit() with a with a reflective, transparent material")
{
    World w = default_world();

    auto floor = std::make_shared<Plane>(Plane());
    floor->set_transform(translation(0, -1, 0));
    Material& m1 = floor->material;
    m1.transparency = 0.5;
    m1.reflective = 0.5;
    m1.refractive_index = 1.5;
    w.shapes.emplace_back(floor);

    auto ball = std::make_shared<Sphere>(Sphere());
    ball->set_transform(translation(0, -3.5, -0.5));
    Material m2 = ball->material;
    m2.color = Color(1, 0, 0);
    m2.ambient = 0.5;
    ball->set_material(m2);
    w.shapes.emplace_back(ball);

    Ray r{Point{0, 0, -3}, Vector{0, -M_SQRT2/2, M_SQRT2/2}};
    Intersections xs{{M_SQRT2, floor.get()}};

    Computations comps = prepare_computations(xs[0], r, xs);
    Color color = shade_hit(w, comps, LightingModel::Phong, 5);
    REQUIRE(color == Color(0.93391, 0.69643, 0.69243));
}
