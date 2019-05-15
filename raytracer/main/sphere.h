#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include <cmath>
#include "ray.h"
#include "point.h"
#include "vector.h"
#include "object.h"
#include "intersection.h"

class Sphere : public Object
{
public:
    Sphere() = default;

    inline std::vector<double> intersect(const Ray& ray) const {

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
        return {t1, t2};
    }

    inline Intersection intersection(double t) {
        return Intersection(t, *this);
    }
};

#endif // SPHERE_H
