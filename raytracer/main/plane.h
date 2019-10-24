#ifndef PLANE_H
#define PLANE_H

#include <vector>
#include <cmath>
#include <memory>
#include "ray.h"
#include "point.h"
#include "vector.h"
#include "shape.h"
#include "matrix.h"
#include "intersection.h"

struct Plane : public Shape
{
    inline Vector local_normal_at(const Point& local_point) const override {
        (void)local_point;
        return {0, 1, 0};
    }

    inline std::vector<Intersection> local_intersect(const Ray& ray) const override {
        // Ray is parallel to the plane
        if (std::abs(ray.direction().y) < EPSILON) {
            return {};
        }

        const float t = -ray.origin().y / ray.direction().y;
        return {Intersection(t, this)};
    }
};

#endif // PLANE_H
