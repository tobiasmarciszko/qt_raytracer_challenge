#include "catch.hpp"
#include "cube.h"
#include "shape.h"
#include "equal.h"

TEST_CASE("A ray intersects a cube")
{
    const std::vector<std::tuple<Point, Vector, int, int>> data{
        {Point{5, 0.5, 0},  Vector{-1, 0, 0}, 4, 6}, // +x
        {Point{-5, 0.5, 0}, Vector{1, 0, 0},  4, 6}, // -x
        {Point{0.5, 5, 0},  Vector{0, -1, 0}, 4, 6}, // +y
        {Point{0.5, -5, 0}, Vector{0, 1, 0},  4, 6}, // -y
        {Point{0.5, 0, 5},  Vector{0, 0, -1}, 4, 6}, // +z
        {Point{0.5, 0, -5}, Vector{0, 0, 1},  4, 6}, // -z
        {Point{0, 0.5, 0},  Vector{0, 0, 1}, -1, 1}  // inside
        };

    Cube c;
    for (const auto& [origin, direction, t1, t2]: data) {
        Ray r{origin, direction};
        auto xs = c.local_intersect(r);
        REQUIRE(xs.size() == 2);
        REQUIRE(equal(xs.at(0).t, t1));
        REQUIRE(equal(xs.at(1).t, t2));
    }
}
