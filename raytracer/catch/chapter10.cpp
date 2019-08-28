#include "catch.hpp"
#include "pattern.h"
#include "material.h"
#include "vector.h"
#include "lighting.h"

TEST_CASE("Creating a stripe pattering")
{
    auto pattern = stripe_pattern(white, black);

    REQUIRE(pattern.a == white);
    REQUIRE(pattern.b == black);
}

TEST_CASE("A stripe pattern is constant in y")
{
    auto pattern = stripe_pattern(white, black);

    REQUIRE(pattern.stripe_at(Point(0,0,0)) == white);
    REQUIRE(pattern.stripe_at(Point(0,1,0)) == white);
    REQUIRE(pattern.stripe_at(Point(0,2,0)) == white);
}

TEST_CASE("A stripe pattern is constant in z")
{
    auto pattern = stripe_pattern(white, black);

    REQUIRE(pattern.stripe_at(Point(0,0,0)) == white);
    REQUIRE(pattern.stripe_at(Point(0,0,1)) == white);
    REQUIRE(pattern.stripe_at(Point(0,0,2)) == white);
}

TEST_CASE("A stripe pattern alternates in x")
{
    auto pattern = stripe_pattern(white, black);

    REQUIRE(pattern.stripe_at(Point(0,0,0)) == white);
    REQUIRE(pattern.stripe_at(Point(0.9,0,0)) == white);
    REQUIRE(pattern.stripe_at(Point(1,0,0)) == black);
    REQUIRE(pattern.stripe_at(Point(-0.1,0,0)) == black);
    REQUIRE(pattern.stripe_at(Point(-1,0,0)) == black);
    REQUIRE(pattern.stripe_at(Point(-1.1,0,0)) == white);
}

TEST_CASE("Lighting with a pattern applied")
{
    Material m;
    m.pattern = stripe_pattern(Color(1,1,1), Color(0,0,0));
    m.ambient = 1;
    m.diffuse = 0;
    m.specular = 0;

    auto eyev = Vector(0,0,-1);
    auto normalv = Vector(0,0,-1);
    auto light = PointLight(Point(0,0,-10), Color(1,1,1));
    auto c1 = lighting(m, light, Point(0.9,0,0), eyev, normalv, false);
    auto c2 = lighting(m, light, Point(1.1,0,0), eyev, normalv, false);

    REQUIRE(c1 == Color(1,1,1));
    REQUIRE(c2 == Color(0,0,0));
}
