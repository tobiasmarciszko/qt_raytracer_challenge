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

TEST_CASE("A ray misses a cube")
{
    const std::vector<std::tuple<Point, Vector>> data{
        {Point{-2, 0,  0}, Vector{0.2673, 0.5345, 0.8018}},
        {Point{0, -2,  0}, Vector{0.8018, 0.2673, 0.5345}},
        {Point{0,  0, -2}, Vector{0.5345 ,0.8018, 0.2673}},
        {Point{2,  0,  2}, Vector{0, 0, -1}},
        {Point{0,  2,  2}, Vector{0, -1, 0}},
        {Point{2,  2,  0}, Vector{-1, 0, 0}},
    };

    Cube c;
    for (const auto& [origin, direction]: data) {
        Ray r{origin, direction};
        auto xs = c.local_intersect(r);
        REQUIRE(xs.size() == 0);
    }
}

TEST_CASE("The normal on the surface of a cube") {
    const std::vector<std::tuple<Point, Vector>> data{
        {Point{1, 0.5, -0.8}, Vector{1, 0, 0}},
        {Point{-1, -0.2, 0.9}, Vector{-1, 0, 0}},
        {Point{-0.4, 1, -0.1}, Vector{0, 1, 0}},
        {Point{0.3, -1, -0.7}, Vector{0, -1, 0}},
        {Point{-0.6, 0.3, 1}, Vector{0, 0, 1}},
        {Point{0.4, 0.4, -1}, Vector{0, 0, -1}},
        {Point{1, 1, 1}, Vector{1, 0, 0}},
        {Point{-1, -1, -1}, Vector{-1, 0, 0}},
    };

    Cube c;
    for (const auto& [point, expected_normal]: data) {

        auto normal = c.local_normal_at(point);
        REQUIRE(normal == expected_normal);
    }
}