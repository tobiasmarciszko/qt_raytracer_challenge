#include "cube.h"
#include "intersection.h"
#include "vector.h"
#include <algorithm>
#include <cmath>

Vector Cube::local_normal_at(const Point& local_point) const {
    return local_point - Point(0, 0, 0);
}

std::vector<Intersection> Cube::local_intersect(const Ray& ray) const {

    const auto [xtmin, xtmax] = check_axis(ray.origin().x, ray.direction().x);
    const auto [ytmin, ytmax] = check_axis(ray.origin().y, ray.direction().y);
    const auto [ztmin, ztmax] = check_axis(ray.origin().z, ray.direction().z);

    const auto tmin = std::max({xtmin, ytmin, ztmin});
    const auto tmax = std::min({xtmax, ytmax, ztmax});

    if (tmin > tmax) return {};

    return {Intersection(tmin, this), Intersection(tmax, this)};
}

std::tuple<float, float> Cube::check_axis(float origin, float direction) const {
    float tmin;
    float tmax;

    const auto tmin_numerator = (-1 - origin);
    const auto tmax_numerator = (1 - origin);

    if (std::abs(direction) >= EPSILON) {
        tmin = tmin_numerator / direction;
        tmax = tmax_numerator / direction;
    } else {
        tmin = tmin_numerator * INFINITY;
        tmax = tmax_numerator * INFINITY;
    }

    if (tmin > tmax) {
        std::swap(tmin, tmax);
    }

    return std::make_tuple(tmin, tmax);
}
