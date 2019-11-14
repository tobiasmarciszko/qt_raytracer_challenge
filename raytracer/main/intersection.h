#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "equal.h"

struct Shape;
struct Intersection
{
    Intersection() = delete;
    Intersection(float _t, const Shape* _object) :
        t(_t),
        object(_object) {}

    inline bool operator==(const Intersection& i1) const {
        if (!(equal(i1.t, t))) return false;
        if (!(i1.object == object)) return false;

        return true;
    }

    float t;
    const Shape* object;
};

#endif // INTERSECTION_H
