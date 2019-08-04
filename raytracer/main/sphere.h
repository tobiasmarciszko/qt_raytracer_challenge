#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include <cmath>
#include "ray.h"
#include "point.h"
#include "vector.h"
#include "shape.h"
#include "matrix.h"
#include "intersection.h"

struct Sphere : public Shape
{
    inline Vector local_normal_at(const Point& local_point) const override {
        return local_point - Point(0, 0, 0);
    }

    inline std::vector<Intersection> local_intersect(const Ray& ray) const override {

        const Vector sphere_to_ray = ray.origin() - Point(0, 0, 0);
        const double a = ray.direction().dot(ray.direction());
        const double b = 2 * ray.direction().dot(sphere_to_ray);
        const double c = sphere_to_ray.dot(sphere_to_ray) - 1.0;

        const double discriminant = (b * b) - (4.0 * a * c);

        // No hit
        if (discriminant < 0) return {};

        // Hit, either two intersections (discriminant == 0)
        // or one on the tanget
        const double t1 = (-b - std::sqrt(discriminant)) / ( 2.0 * a);
        const double t2 = (-b + std::sqrt(discriminant)) / ( 2.0 * a);
        const auto sphere_ptr = std::make_shared<const Sphere>(*this);
        return {intersection(t1, sphere_ptr), intersection(t2, sphere_ptr)};
    }
};

#endif // SPHERE_H
