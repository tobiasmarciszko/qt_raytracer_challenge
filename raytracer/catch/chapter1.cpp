#include "catch.hpp"
#include "point.h"
#include "tuple.h"
#include "point.h"
#include "vector.h"
#include "equal.h"

TEST_CASE("Test Tuple") {
    Tuple tuple(1.0, 2.0, 3.0, 4.0);

    REQUIRE(equal(1.0, tuple.x));
    REQUIRE(equal(2.0, tuple.y));
    REQUIRE(equal(3.0, tuple.z));
    REQUIRE(equal(4.0, tuple.w));
}

TEST_CASE("testPoint")
{
    Point point = Point(1, 2, 3);
    REQUIRE(point.isPoint());

    Tuple tuple = point;
    REQUIRE(equal(1, tuple.w));
}

TEST_CASE("testVector")
{
    Vector vector = Vector(1, 2, 3);
    REQUIRE(vector.isVector());

    Tuple tuple = vector;
    REQUIRE(equal(0, tuple.w));
}

TEST_CASE("testAdd")
{
    Tuple a1 = Tuple(3, -2, 5, 1);
    Tuple a2 = Tuple(-2, 3, 1, 0);

    Tuple a3 = a1 + a2;

    REQUIRE(equal(1, a3.x));
    REQUIRE(equal(1, a3.y));
    REQUIRE(equal(6, a3.z));
    REQUIRE(equal(1, a3.w));
}

TEST_CASE("testSubtractPoints")
{
    Point p1 = Point(3, 2, 1);
    Point p2 = Point(5, 6, 7);

    Vector v1 = p1 - p2;

    REQUIRE(v1.isVector());
    REQUIRE(equal(-2, v1.x));
    REQUIRE(equal(-4, v1.y));
    REQUIRE(equal(-6, v1.z));
}

TEST_CASE("testSubtractVectorFromPoint")
{
    Point p = Point(3, 2, 1);
    Vector v = Vector(5, 6, 7);

    Point p1 = p - v;

    REQUIRE(p1.isPoint());
    REQUIRE(equal(-2, p1.x));
    REQUIRE(equal(-4, p1.y));
    REQUIRE(equal(-6, p1.z));
}

TEST_CASE("testSubtractVectors")
{
    Vector v1 = Vector(3, 2, 1);
    Vector v2 = Vector(5, 6, 7);

    Vector v3 = v1 - v2;

    REQUIRE(v3.isVector());
    REQUIRE(equal(-2, v3.x));
    REQUIRE(equal(-4, v3.y));
    REQUIRE(equal(-6, v3.z));
}

TEST_CASE("testNegate")
{
    Tuple tuple = -Tuple(1, -2, 3, -4);

    REQUIRE(equal(-1, tuple.x));
    REQUIRE(equal(2, tuple.y));
    REQUIRE(equal(-3, tuple.z));
    REQUIRE(equal(4, tuple.w));
}

TEST_CASE("testScalarMultiply")
{
    Tuple a = Tuple(1, -2, 3, -4);
    Tuple tuple = a * 3.5;

    REQUIRE(equal(3.5, tuple.x));
    REQUIRE(equal(-7, tuple.y));
    REQUIRE(equal(10.5, tuple.z));
    REQUIRE(equal(-14, tuple.w));
}

TEST_CASE("testFractionMultiply")
{
    Tuple a = Tuple(1, -2, 3, -4);
    Tuple tuple = a * 0.5;

    REQUIRE(equal(0.5, tuple.x));
    REQUIRE(equal(-1, tuple.y));
    REQUIRE(equal(1.5, tuple.z));
    REQUIRE(equal(-2, tuple.w));
}

TEST_CASE("testScalarDivision")
{
    Tuple a = Tuple(1, -2, 3, -4);
    Tuple tuple = a / 2;

    REQUIRE(equal(0.5, tuple.x));
    REQUIRE(equal(-1, tuple.y));
    REQUIRE(equal(1.5, tuple.z));
    REQUIRE(equal(-2, tuple.w));
}

TEST_CASE("testMagnitude")
{
    Vector v1 = Vector(1,0,0);
    REQUIRE(equal(1, v1.magnitude()));

    Vector v2 = Vector(0,1,0);
    REQUIRE(equal(1, v2.magnitude()));

    Vector v3 = Vector(0,0,1);
    REQUIRE(equal(1, v3.magnitude()));

    Vector v4 = Vector(1,2,3);
    REQUIRE(equal(sqrt(14), v4.magnitude()));

    Vector v5 = Vector(-1,-2,-3);
    REQUIRE(equal(sqrt(14), v5.magnitude()));
}

TEST_CASE("testNormalize")
{
    Vector v1 = Vector(4,0,0);
    Vector v2 = v1.normalize();

    REQUIRE(equal(1, v2.x));

    Vector v3 = Vector(1,2,3);
    Vector v4 = v3.normalize();

    REQUIRE(equal(1 / sqrt(14), v4.x));
    REQUIRE(equal(2 / sqrt(14), v4.y));
    REQUIRE(equal(3 / sqrt(14), v4.z));
}

TEST_CASE("testDot")
{
    Vector a = Vector(1,2,3);
    Vector b = Vector(2,3,4);

    const float dot = a.dot(b);

    REQUIRE(equal(20, dot));
}

TEST_CASE("testCross")
{
    Vector a = Vector(1,2,3);
    Vector b = Vector(2,3,4);

    Vector cross1 = a.cross(b);
    REQUIRE(equal(-1, cross1.x));
    REQUIRE(equal(2, cross1.y));
    REQUIRE(equal(-1, cross1.z));

    Vector cross2 = b.cross(a);
    REQUIRE(equal(1, cross2.x));
    REQUIRE(equal(-2, cross2.y));
    REQUIRE(equal(1, cross2.z));

}
