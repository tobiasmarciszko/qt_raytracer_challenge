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

class Sphere : public Shape
{
public:
    inline Vector normal_at(const Point& world_point) const override {
        const Point object_point = m_transform.inverse() * world_point;
        const Vector object_normal = object_point - Point(0, 0, 0);
        Vector world_normal = m_transform.inverse().transpose() * object_normal;

        // Transpose messes with our w value. Hack it back to 0;
        world_normal.resetWToZero();
        return world_normal.normalize();
    }

    inline std::vector<Intersection> intersect(const Ray& r) const override {

        const auto ray = r.transform(m_transform.inverse());

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
        return {intersection(t1, std::make_shared<Sphere>(*this)), intersection(t2, std::make_shared<Sphere>(*this))};
    }

};

#endif // SPHERE_H
