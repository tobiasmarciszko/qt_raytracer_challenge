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
#include "intersection.h"

TEST_CASE("testRayCreation")
{
    const auto origin = Point(1, 2, 3);
    const auto direction = Vector(4, 5, 6);

    const auto r = Ray(origin, direction);

    REQUIRE(r.origin() == origin);
    REQUIRE(r.direction() == direction);
}

TEST_CASE("testRayPosition")
{
    const auto r = Ray(Point(2, 3, 4), Vector(1, 0, 0));

    REQUIRE(r.position(0) == Point(2, 3, 4));
    REQUIRE(r.position(1) == Point(3, 3, 4));
    REQUIRE(r.position(-1) == Point(1, 3, 4));
    REQUIRE(r.position(2.5) == Point(4.5, 3, 4));
}

TEST_CASE("testRayIntersectsSphereAtTwoPoints")
{
    const auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
    const auto s = Sphere();

    const auto xs = s.intersect(r);

    REQUIRE(xs.size() == 2);
    REQUIRE(equal(xs.at(0).t, 4.0));
    REQUIRE(equal(xs.at(1).t, 6.0));
}

TEST_CASE("testRayIntersectsSphereAtTangent")
{
    const auto r = Ray(Point(0, 1, -5), Vector(0, 0, 1));
    const auto s = Sphere();

    const auto xs = s.intersect(r);

    REQUIRE(xs.size() == 2);
    REQUIRE(equal(xs.at(0).t, 5.0));
    REQUIRE(equal(xs.at(1).t, 5.0));
}

TEST_CASE("testRayMissesSphere")
{
    const auto r = Ray(Point(0, 2, -5), Vector(0, 0, 1));
    const auto s = Sphere();

    const auto xs = s.intersect(r);

    REQUIRE(xs.empty());
}

TEST_CASE("testRayOriginatesInsideSphere")
{
    const auto r = Ray(Point(0, 0, 0), Vector(0, 0, 1));
    const auto s = Sphere();

    const auto xs = s.intersect(r);

    REQUIRE(xs.size() == 2);
    REQUIRE(equal(xs.at(0).t, -1.0));
    REQUIRE(equal(xs.at(1).t, 1.0));
}

TEST_CASE("testRayIsInFrontOfSphere")
{
    const auto r = Ray(Point(0, 0, 5), Vector(0, 0, 1));
    const auto s = Sphere();

    const auto xs = s.intersect(r);

    REQUIRE(xs.size() == 2);
    REQUIRE(equal(xs.at(0).t, -6.0));
    REQUIRE(equal(xs.at(1).t, -4.0));
}

TEST_CASE("Spheres should have unique ids") {
    const auto s1 = Sphere();
    const auto s2 = Sphere();

    REQUIRE(s1.id() != s2.id());
}

TEST_CASE("An intersection encapsulates t and object") {
    const auto s = std::make_shared<Sphere>(Sphere()).get();
    const auto i = Intersection(3.5, s);
    REQUIRE(equal(i.t, 3.5));
    REQUIRE(s == i.object);
}

TEST_CASE("Aggregating intersections") {
    const auto s = std::make_shared<Sphere>(Sphere()).get();
    const auto i1 = Intersection(1, s);
    const auto i2 = Intersection(2, s);

    const auto xs = Intersections({i1, i2});

    REQUIRE(xs.size() == 2);
    REQUIRE(equal(xs.at(0).t, 1));
    REQUIRE(equal(xs.at(1).t, 2));
}

TEST_CASE("Intersect sets the object on the intersection") {
    const auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
    const auto s = Sphere();
    const auto xs = s.intersect(r);

    REQUIRE(xs.size() == 2);

    REQUIRE(*xs.at(0).object == s);
    REQUIRE(*xs.at(1).object == s);
}

TEST_CASE("The hit, when all intersections have positive t") {
    const auto s = std::make_shared<Sphere>(Sphere()).get();
    const auto i1 = Intersection(1, s);
    const auto i2 = Intersection(2, s);

    const auto xs = Intersections({i1, i2});
    const auto i = hit(xs);

    REQUIRE(i == i1);
}

TEST_CASE("The hit, when some intersections have negative t") {
    const auto s = std::make_shared<Sphere>(Sphere()).get();
    const auto i1 = Intersection(-1, s);
    const auto i2 = Intersection(1, s);

    const auto xs = Intersections({i1, i2});
    const auto i = hit(xs);

    REQUIRE(i == i2);
}

TEST_CASE("The hit, when all intersections have negative t") {
    const auto s = std::make_shared<Sphere>(Sphere()).get();
    const auto i1 = Intersection(-2, s);
    const auto i2 = Intersection(-1, s);

    const auto xs = Intersections({i1, i2});
    const auto i = hit(xs);

    REQUIRE(i.has_value() == false);
}

TEST_CASE("The hit is always the lowest non negative intersection") {
    const auto s = std::make_shared<Sphere>(Sphere()).get();

    const auto i1 = Intersection(5, s);
    const auto i2 = Intersection(7, s);
    const auto i3 = Intersection(-3, s);
    const auto i4 = Intersection(2, s);

    const auto xs = Intersections({i1, i2, i3, i4});
    const auto i = hit(xs);

    REQUIRE(i == i4);
}

TEST_CASE("Translating a ray") {
    const auto r = Ray(Point(1, 2, 3), Vector(0, 1, 0));
    const auto m = translation(3, 4, 5);
    const auto r2 = r.transform(m);

    REQUIRE(r2.origin() == Point(4, 6, 8));
    REQUIRE(r2.direction() == Vector(0, 1, 0));
}

TEST_CASE("Scaling a ray") {
    const auto r = Ray(Point(1, 2, 3), Vector(0, 1, 0));
    const auto m = scaling(2, 3, 4);
    const auto r2 = r.transform(m);

    REQUIRE(r2.origin() == Point(2, 6, 12));
    REQUIRE(r2.direction() == Vector(0, 3, 0));
}

TEST_CASE("A sphere's default transformation") {
    const auto s = Sphere();

    REQUIRE(s.transform() == identity_matrix);
}

TEST_CASE("Changing a sphere's transformation") {
    auto s = Sphere();
    const auto t = translation(2, 3, 4);
    s.set_transform(t);

    REQUIRE(s.transform() == t);
}

TEST_CASE("Intersecting a scaled sphere with a ray") {
    const auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
    auto s = Sphere();
    s.set_transform(scaling(2, 2, 2));
    const auto xs = s.intersect(r);

    REQUIRE(xs.size() == 2);
    REQUIRE(xs.at(0).t == 3);
    REQUIRE(xs.at(1).t == 7);
}

TEST_CASE("Intersecting a translated sphere with a ray") {
    const auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
    auto s = Sphere();
    s.set_transform(translation(5, 0, 0));
    const auto xs = s.intersect(r);

    REQUIRE(xs.empty());
}
