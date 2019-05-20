#include "catch.hpp"
#include "tuple.h"
#include "point.h"
#include "tuple.h"
#include "point.h"
#include "vector.h"
#include "color.h"
#include "canvas.h"

TEST_CASE("testColor")
{
    Color c = Color(-0.5, 0.4, 1.7);

    REQUIRE(equal(-0.5, c.red()));
    REQUIRE(equal(0.4, c.green()));
    REQUIRE(equal(1.7, c.blue()));
}

TEST_CASE("testColorAdd")
{
    Color c1 = Color(0.9, 0.6, 0.75);
    Color c2 = Color(0.7, 0.1, 0.25);

    Color c3 = c1 + c2;

    REQUIRE(equal(1.6, c3.red()));
    REQUIRE(equal(0.7, c3.green()));
    REQUIRE(equal(1.0, c3.blue()));
}

TEST_CASE("testColorSubtract")
{
    Color c1 = Color(0.9, 0.6, 0.75);
    Color c2 = Color(0.7, 0.1, 0.25);

    Color c3 = c1 - c2;

    REQUIRE(equal(0.2, c3.red()));
    REQUIRE(equal(0.5, c3.green()));
    REQUIRE(equal(0.5, c3.blue()));
}

TEST_CASE("testColorScalar")
{
    Color c1 = Color(0.2, 0.3, 0.4);
    Color c2 = c1 * 2;

    REQUIRE(equal(0.4, c2.red()));
    REQUIRE(equal(0.6, c2.green()));
    REQUIRE(equal(0.8, c2.blue()));
}

TEST_CASE("testColorHadamard") {

    Color c1 = Color(1, 0.2, 0.4);
    Color c2 = Color(0.9, 1, 0.1);

    Color c3 = c1 * c2;

    REQUIRE(equal(0.9, c3.red()));
    REQUIRE(equal(0.2, c3.green()));
    REQUIRE(equal(0.04, c3.blue()));
}

TEST_CASE("testCanvas") {
    Canvas<10,20> c;
    Color red = Color(1, 0, 0);

    c.write_pixel(0, 0, red);
    REQUIRE(equal(1, c.pixel_at(0, 0).red()));

    c.write_pixel(10 - 1, 20 - 1, red);
    REQUIRE(equal(1, c.pixel_at(10 - 1, 20 - 1).red()));
}
