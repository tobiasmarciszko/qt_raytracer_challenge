#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "object.h"

#include <vector>
#include <algorithm>
#include <optional>
#include <initializer_list>

class Intersection
{
public:

    Intersection(double t, const Object& object) :
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

    inline Object object() const {
        return m_object;
    }

private:
    double m_t;
    Object m_object;
};

inline std::vector<Intersection> intersections(std::initializer_list<Intersection> l) {
    std::vector<Intersection> v;
    v.insert(v.end(), l.begin(), l.end());
    return v;
}

inline std::optional<Intersection> hit(std::vector<Intersection> intersections) {

    // sort the list of intersections and return the first intersection
    // with a non negative t value
    //
    // if no such value exists, return an empty Intersection

    std::sort(intersections.begin(), intersections.end(), [](
        const Intersection& i1,
        const Intersection& i2) -> bool {
            return i1.t() < i2.t();
    });

    for (const auto& intersection: intersections) {
        if (intersection.t() >= 0) {
            return {intersection};
        }
    }

    return {};
}

#endif // INTERSECTION_H
