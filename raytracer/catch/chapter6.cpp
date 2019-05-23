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

TEST_CASE("The normal on a sphere at a point on the x axis") {
    const auto s = Sphere();
    const auto n = s.normal_at(Point(1, 0, 0));

    REQUIRE(n == Vector(1, 0, 0));
}

TEST_CASE("The normal on a sphere at a point on the y axis") {
    const auto s = Sphere();
    const auto n = s.normal_at(Point(0, 1, 0));

    REQUIRE(n == Vector(0, 1, 0));
}

TEST_CASE("The normal on a sphere at a point on the z axis") {
    const auto s = Sphere();
    const auto n = s.normal_at(Point(0, 0, 1));

    REQUIRE(n == Vector(0, 0, 1));
}

TEST_CASE("The normal on a sphere at a nonaxial point") {
    const double val = std::sqrt(3.0)/3.0;

    const auto s = Sphere();
    const auto n = s.normal_at(Point(val, val, val));

    REQUIRE(n == Vector(val, val, val));
}

TEST_CASE("The normal is a normalized vector") {
    const double val = std::sqrt(3.0)/3.0;

    const auto s = Sphere();
    const auto n = s.normal_at(Point(val, val, val));

    REQUIRE(n == n.normalize());
}
