#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <vector>
#include <algorithm>
#include <optional>
#include "sphere.h"

class Intersection
{
public:
    Intersection(double t, const Sphere& object) :
        m_t(t),
        m_object(object) {}

    inline bool operator==(const Intersection& i1) const {
        if (i1.t() != m_t) return false;
        if (!(i1.object() == m_object)) return false;

        return true;
    }

    inline double t() const {
        return m_t;
    }

    inline Sphere object() const {
        return m_object;
    }

private:
    double m_t;
    Sphere m_object;
};

using Intersections = std::vector<Intersection>;


inline Intersection intersection(double t, const Sphere& s) {
    return {t,s};
}

inline std::vector<Intersection> intersect(const Ray& r, const Sphere& s) {

    const auto ray = r.transform(s.transform().inverse());

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
    return {intersection(t1, s), intersection(t2, s)};
}

inline std::optional<Intersection> hit(Intersections intersections) {

    // sort the list of intersections and return the first intersection
    // with a non negative t value
    //
    // if no such value exists, return an empty Intersection

    std::sort(intersections.begin(), intersections.end(), [](
        const Intersection& i1,
        const Intersection& i2) -> bool {
            return i1.t() < i2.t();
    });

    for (const Intersection& intersection: intersections) {
        if (intersection.t() >= 0) {
            return {intersection};
        }
    }

    return {};
}

#endif // INTERSECTION_H
