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
