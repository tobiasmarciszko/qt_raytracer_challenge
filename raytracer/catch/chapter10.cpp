#include "catch.hpp"
#include "pattern.h"
#include "material.h"
#include "vector.h"
#include "sphere.h"
#include "shape.h"
#include "light.h"
#include "engine.h"

using namespace Raytracer::Engine;

TEST_CASE("A stripe pattern is constant in y")
{
    auto pattern = stripe_pattern(white, black);

    REQUIRE(pattern->pattern_at(Point(0,0,0)) == white);
    REQUIRE(pattern->pattern_at(Point(0,1,0)) == white);
    REQUIRE(pattern->pattern_at(Point(0,2,0)) == white);
}

TEST_CASE("A stripe pattern is constant in z")
{
    auto pattern = stripe_pattern(white, black);

    REQUIRE(pattern->pattern_at(Point(0,0,0)) == white);
    REQUIRE(pattern->pattern_at(Point(0,0,1)) == white);
    REQUIRE(pattern->pattern_at(Point(0,0,2)) == white);
}

TEST_CASE("A stripe pattern alternates in x")
{
    auto pattern = stripe_pattern(white, black);

    REQUIRE(pattern->pattern_at(Point(0,0,0)) == white);
    REQUIRE(pattern->pattern_at(Point(0.9,0,0)) == white);
    REQUIRE(pattern->pattern_at(Point(1,0,0)) == black);
    REQUIRE(pattern->pattern_at(Point(-0.1,0,0)) == black);
    REQUIRE(pattern->pattern_at(Point(-1,0,0)) == black);
    REQUIRE(pattern->pattern_at(Point(-1.1,0,0)) == white);
}

TEST_CASE("Lighting with a pattern applied")
{
    Material m;
    m.pattern_ptr = stripe_pattern(Color(1,1,1), Color(0,0,0));
    m.ambient = 1;
    m.diffuse = 0;
    m.specular = 0;

    auto eyev = Vector(0,0,-1);
    auto normalv = Vector(0,0,-1);
    auto light = PointLight(Point(0,0,-10), Color(1,1,1));

    const auto object = std::make_shared<Sphere>().get();

    auto c1 = lighting(m, object, light, Point(0.9,0,0), eyev, normalv, false);
    auto c2 = lighting(m, object, light, Point(1.1,0,0), eyev, normalv, false);

    REQUIRE(c1 == Color(1,1,1));
    REQUIRE(c2 == Color(0,0,0));
}

TEST_CASE("Stripes with an object transformation")
{
    const auto object = std::make_shared<Sphere>().get();
    object->set_transform(scaling(2, 2, 2));

    auto pattern = stripe_pattern(white, black);
    auto c = pattern_at_shape(pattern, object, Point(1.5, 0, 0));

    REQUIRE(c == white);
}

TEST_CASE("Stripes with a pattern transformation")
{
    const auto object = std::make_shared<Sphere>().get();
    object->set_transform(scaling(2, 2, 2));

    auto pattern = stripe_pattern(white, black);
    pattern->set_transform(scaling(2, 2, 2));
    auto c = pattern_at_shape(pattern, object, Point(1.5, 0, 0));

    REQUIRE(c == white);

}

TEST_CASE("Stripes with both an object and a pattern transformation")
{
    const auto object = std::make_shared<Sphere>().get();
    object->set_transform(scaling(2, 2, 2));

    auto pattern = stripe_pattern(white, black);
    pattern->set_transform(scaling(2, 2, 2));
    auto c = pattern_at_shape(pattern, object, Point(1.5, 0, 0));

    REQUIRE(c == white);
}
