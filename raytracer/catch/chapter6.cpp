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
#include "lighting.h"

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

TEST_CASE("Computing the normal on a translated sphere") {
    auto s = Sphere();

    s.set_transform(translation(0, 1, 0));

    const auto n = s.normal_at(Point(0, 1.70711, -0.70711));
    REQUIRE(n == Vector(0, 0.70711, -0.70711));
}

TEST_CASE("Computing the normal on a transformed sphere") {
    auto s = Sphere();

    s.set_transform(scaling(1, 0.5, 1) * rotation_y(M_PI/5));

    const auto n = s.normal_at(Point(0, M_SQRT2/2, -M_SQRT2/2));
    REQUIRE(n == Vector(0, 0.97014, -0.24254));
}

TEST_CASE("Reflecting a vector approaching at 45°") {
    const auto v = Vector(1, -1, 0);
    const auto n = Vector(0, 1, 0);
    const auto r = v.reflect(n);

    REQUIRE(r == Vector(1, 1, 0));
}

TEST_CASE("Reflecting a vector off a slanted surface") {
    const auto v = Vector(0, -1, 0);
    const auto n = Vector(M_SQRT2/2, M_SQRT2/2, 0);
    const auto r = v.reflect(n);

    REQUIRE(r == Vector(1, 0, 0));
}

TEST_CASE("A point light has a position and intensity") {
    const auto intensity = Color(1, 1, 1);
    const auto position = Point(0, 0, 0);

    const auto light = PointLight(position, intensity);

    REQUIRE(light.position() == position);
    REQUIRE(light.intensity() == intensity);
}

TEST_CASE("The default material") {
    const auto m = Material();

    REQUIRE(m.color == Color(1, 1, 1));
    REQUIRE(equal(m.ambient, 0.1));
    REQUIRE(equal(m.diffuse, 0.9));
    REQUIRE(equal(m.specular, 0.9));
    REQUIRE(equal(m.shininess, 200.0));
}

TEST_CASE("A sphere has a default material") {
    const auto s = Sphere();
    const auto m = s.material();

    REQUIRE(m == Material());
}

TEST_CASE("A sphere may be assigned a material") {
    auto s = Sphere();
    auto m = Material();
    m.ambient = 1.0;
    s.set_material(m);

    REQUIRE(s.material() == m);
}

SCENARIO("Lighting") {

    const auto m = Material();
    const auto position = Point(0, 0, 0);

    GIVEN("Lighting with the eye between the light and the surface")
    {
        const auto eyev = Vector(0, 0, -1);
        const auto normalv = Vector(0, 0, -1);
        const auto light = PointLight(Point(0, 0, -10), Color(1, 1, 1));

        const auto result = lighting(m, light, position, eyev, normalv);

        REQUIRE(result == Color(1.9, 1.9, 1.9));
    }

    GIVEN("Lighting with the eye between light and surface, eye offset 45°")
    {
        const auto eyev = Vector(0, M_SQRT2/2, M_SQRT2/2);
        const auto normalv = Vector(0, 0, -1);
        const auto light = PointLight(Point(0, 0, -10), Color(1, 1, 1));

        const auto result = lighting(m, light, position, eyev, normalv);

        REQUIRE(result == Color(1.0, 1.0, 1.0));
    }

    GIVEN("Lighting with eye opposite surface, light offset 45°")
    {
        const auto eyev = Vector(0, 0, -1);
        const auto normalv = Vector(0, 0, -1);
        const auto light = PointLight(Point(0, 10, -10), Color(1, 1, 1));

        const auto result = lighting(m, light, position, eyev, normalv);

        REQUIRE(result == Color(0.7364, 0.7364, 0.7364));
    }

    GIVEN("Lighting with eye in the path of the reflection vector")
    {
        const auto eyev = Vector(0, -M_SQRT2/2, -M_SQRT2/2);
        const auto normalv = Vector(0, 0, -1);
        const auto light = PointLight(Point(0, 10, -10), Color(1, 1, 1));

        const auto result = lighting(m, light, position, eyev, normalv);

        REQUIRE(result == Color(1.6364, 1.6364, 1.6364));
    }

    GIVEN("Lighting with the light behind the surface")
    {
        const auto eyev = Vector(0, 0, -1);
        const auto normalv = Vector(0, 0, -1);
        const auto light = PointLight(Point(0, 0, 10), Color(1, 1, 1));

        const auto result = lighting(m, light, position, eyev, normalv);

        REQUIRE(result == Color(0.1, 0.1, 0.1));
    }

}



