#include "catch.hpp"
#include "plane.h"

TEST_CASE("The normal of a plane is constant everywhere")
{
    const Plane p;

    const auto n1 = p.local_normal_at(Point(0, 0, 0));
    const auto n2 = p.local_normal_at(Point(10, 0, -0));
    const auto n3 = p.local_normal_at(Point(-5, 0, 150));

    REQUIRE(n1 == Vector(0, 1, 0));
    REQUIRE(n2 == Vector(0, 1, 0));
    REQUIRE(n3 == Vector(0, 1, 0));
}

TEST_CASE("Intersect with a ray parallel to the plane")
{
    const Plane p;
    const auto r = Ray(Point(0, 10, 0), Vector(0, 0, 1));

    const auto xs = p.local_intersect(r);

    REQUIRE(xs.empty());
}

TEST_CASE("Intersect with a coplanar ray")
{
    const Plane p;
    const auto r = Ray(Point(0, 0, 0), Vector(0, 0, 1));

    const auto xs = p.local_intersect(r);

    REQUIRE(xs.empty());
}


TEST_CASE("A ray intersecting a plane from above")
{
    const Plane p;
    const auto r = Ray(Point(0, 1, 0), Vector(0, -1, 0));

    const auto xs = p.local_intersect(r);

    REQUIRE(xs.size() == 1);
    REQUIRE(equal(xs.at(0).t, 1));
    REQUIRE(xs.at(0).object->id() == p.id());
}


TEST_CASE("A ray intersecting a plane from below")
{
    const Plane p;
    const auto r = Ray(Point(0, -1, 0), Vector(0, 1, 0));

    const auto xs = p.local_intersect(r);

    REQUIRE(xs.size() == 1);
    REQUIRE(equal(xs.at(0).t, 1));
    REQUIRE(xs.at(0).object->id() == p.id());
}
