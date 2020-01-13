#include "catch.hpp"
#include "cylinder.h"
#include "shape.h"
#include "equal.h"

TEST_CASE("A ray misses a cylinder")
{
  Cylinder cyl;

  const std::vector<std::tuple<Point, Vector>> data{
      {Point{1, 0,  0}, Vector{0, 1, 0}},
      {Point{0, 0,  0}, Vector{0, 1, 0}},
      {Point{0,  0, -5}, Vector{1, 1, 1}}
  };

  for (const auto& [origin, direction]: data) {
    const auto dir = direction.normalize();
    const auto r = Ray(origin, dir);
    const auto xs = cyl.local_intersect(r);

    REQUIRE(xs.empty());
  }
}


TEST_CASE("A ray strikes a cylinder")
{
  Cylinder cyl;

  const std::vector<std::tuple<Point, Vector, float, float>> data{
      {Point{1, 0,  -5}, Vector{0, 0, 1}, 5, 5},
      {Point{0, 0,  -5}, Vector{0, 0, 1}, 4, 6},
      {Point{0.5,  0, -5}, Vector{0.1, 1, 1}, 6.80798, 7.08872}
  };

  for (const auto& [origin, direction, t0, t1]: data) {
    const auto dir = direction.normalize();
    const auto r = Ray(origin, dir);
    const auto xs = cyl.local_intersect(r);

    REQUIRE(xs.size() == 2);
    REQUIRE(equal(xs.at(0).t, t0));
    REQUIRE(equal(xs.at(1).t, t1));
  }
}