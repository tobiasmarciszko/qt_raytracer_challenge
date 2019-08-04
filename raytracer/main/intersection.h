#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <vector>
#include <algorithm>
#include <optional>
#include <memory>

struct Shape;
class Intersection
{
public:

    Intersection() = delete;
    Intersection(double _t, std::shared_ptr<const Shape> _object) :
        t(_t),
        object(_object) {}

    inline bool operator==(const Intersection& i1) const {
        if (i1.t != t) return false;
        if (!(i1.object == object)) return false;

        return true;
    }

    double t;
    std::shared_ptr<const Shape> object;
};

using Intersections = std::vector<Intersection>;

inline Intersection intersection(double t, std::shared_ptr<const Shape> s) {
    return {t, s};
}

inline std::optional<Intersection> hit(Intersections intersections) {

    // sort the list of intersections and return the first intersection
    // with a non negative t value
    //
    // if no such value exists, return an empty Intersection

    std::sort(intersections.begin(), intersections.end(), [](
        const Intersection& i1,
        const Intersection& i2) -> bool {
            return i1.t < i2.t;
    });

    for (const Intersection& intersection: intersections) {
        if (intersection.t >= 0) {
            return {intersection};
        }
    }

    return {};
}

#endif // INTERSECTION_H
