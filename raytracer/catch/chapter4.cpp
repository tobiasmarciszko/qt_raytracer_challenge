#include "catch.hpp"
#include "tuple.h"
#include "point.h"
#include "tuple.h"
#include "point.h"
#include "vector.h"
#include "color.h"
#include "canvas.h"
#include "matrix.h"

TEST_CASE("testTranslation")
{
    const auto transform = translation(5, -3, 2);
    const auto p = Point(-3, 4, 5);

    const auto result = transform * p;

    REQUIRE(result == Point(2, 1, 7));
}

TEST_CASE("testInverseTranslation")
{
    const auto transform = translation(5, -3, 2);
    const auto inv = transform.inverse();
    const auto p = Point(-3, 4, 5);

    const auto result = inv * p;

    REQUIRE(result == Point(-8, 7, 3));
}

TEST_CASE("testTranslateVector")
{
    const auto transform = translation(5, -3, 2);
    const auto v = Vector(-3, 4, 5);

    const auto result = transform * v;

    REQUIRE(result == v);
}

TEST_CASE("testScalePoint")
{
    const auto transform = scaling(2, 3, 4);
    const auto p = Point(-4, 6, 8);

    const auto result = transform * p;

    REQUIRE(result == Point(-8, 18, 32));
}

TEST_CASE("testScaleVector")
{
    const auto transform = scaling(2, 3, 4);
    const auto v = Vector(-4, 6, 8);

    const auto result = transform * v;

    REQUIRE(result == Vector(-8, 18, 32));
}

TEST_CASE("testInverseScale")
{
    const auto transform = scaling(2, 3, 4);
    const auto inv = transform.inverse();
    const auto v = Vector(-4, 6, 8);

    const auto result = inv * v;

    REQUIRE(result == Vector(-2, 2, 2));
}

TEST_CASE("testReflection")
{
    const auto transform = scaling(-1, 1, 1);
    const auto p = Point(2, 3, 4);

    const auto result = transform * p;

    REQUIRE(result == Point(-2, 3, 4));
}

TEST_CASE("testRotateX")
{
    const auto p = Point(0, 1, 0);
    const auto half_quarter = rotation_x(M_PI_4);
    const auto full_quarter = rotation_x(M_PI_2);

    REQUIRE(half_quarter * p == Point(0, M_SQRT2 / 2, M_SQRT2 / 2));
    REQUIRE(full_quarter * p == Point(0, 0, 1));
}

TEST_CASE("testRotateInverseX")
{
    const auto p = Point(0, 1, 0);
    const auto half_quarter = rotation_x(M_PI_4);
    const auto inv = half_quarter.inverse();

    REQUIRE(inv * p == Point(0, M_SQRT2 / 2, - M_SQRT2 / 2));
}

TEST_CASE("testRotateY")
{
    const auto p = Point(0, 0, 1);
    const auto half_quarter = rotation_y(M_PI_4);
    const auto full_quarter = rotation_y(M_PI_2);

    REQUIRE(half_quarter * p == Point(M_SQRT2 / 2, 0, M_SQRT2 / 2));
    REQUIRE(full_quarter * p == Point(1, 0, 0));
}

TEST_CASE("testRotateInverseY")
{
    const auto p = Point(0, 0, 1);
    const auto half_quarter = rotation_y(M_PI_4);
    const auto inv = half_quarter.inverse();

    REQUIRE(inv * p == Point(-M_SQRT2 / 2, 0, M_SQRT2 / 2));
}


TEST_CASE("testRotateZ")
{
    const auto p = Point(0, 1, 0);
    const auto half_quarter = rotation_z(M_PI_4);
    const auto full_quarter = rotation_z(M_PI_2);

    REQUIRE(half_quarter * p == Point(-M_SQRT2 / 2, M_SQRT2 / 2, 0));
    REQUIRE(full_quarter * p == Point(-1, 0, 0));
}

TEST_CASE("testRotateInverseZ")
{
    const auto p = Point(0, 1, 0);
    const auto half_quarter = rotation_z(M_PI_4);
    const auto inv = half_quarter.inverse();

    REQUIRE(inv * p == Point(M_SQRT2 / 2, M_SQRT2 / 2, 0));
}

TEST_CASE("testShearingXY")
{
    auto const transform = shearing(1, 0, 0, 0, 0, 0);
    auto const p = Point(2, 3, 4);

    REQUIRE(transform * p == Point(5, 3, 4));
}

TEST_CASE("testShearingXZ")
{
    auto const transform = shearing(0, 1, 0, 0, 0, 0);
    auto const p = Point(2, 3, 4);

    REQUIRE(transform * p == Point(6, 3, 4));
}

TEST_CASE("testShearingYX")
{
    auto const transform = shearing(0, 0, 1, 0, 0, 0);
    auto const p = Point(2, 3, 4);

    REQUIRE(transform * p == Point(2, 5, 4));
}

TEST_CASE("testShearingYZ")
{
    auto const transform = shearing(0, 0, 0, 1, 0, 0);
    auto const p = Point(2, 3, 4);

    REQUIRE(transform * p == Point(2, 7, 4));
}

TEST_CASE("testShearingZX")
{
    auto const transform = shearing(0, 0, 0, 0, 1, 0);
    auto const p = Point(2, 3, 4);

    REQUIRE(transform * p == Point(2, 3, 6));
}

TEST_CASE("testShearingZY")
{
    auto const transform = shearing(0, 0, 0, 0, 0, 1);
    auto const p = Point(2, 3, 4);

    REQUIRE(transform * p == Point(2, 3, 7));
}

TEST_CASE("testTransformationSequence")
{
    const auto p = Point(1, 0, 1);
    const auto A = rotation_x(M_PI_2);
    const auto B = scaling(5, 5, 5);
    const auto C = translation(10, 5, 7);

    // apply rotation
    const auto p2 = A * p;
    REQUIRE(p2 == Point(1, -1, 0));

    // apply scaling
    const auto p3 = B * p2;
    REQUIRE(p3 == Point(5, -5, 0));

    // apply translation
    const auto p4 = C * p3;
    REQUIRE(p4 == Point(15, 0, 7));
}

TEST_CASE("testChainTransformationsInReverseOrder")
{
    const auto p = Point(1, 0, 1);
    const auto A = rotation_x(M_PI_2);
    const auto B = scaling(5, 5, 5);
    const auto C = translation(10, 5, 7);

    const auto T = C * B * A;
    REQUIRE((T * p) == Point(15, 0, 7));
}
