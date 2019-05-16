#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "object.h"

class Intersection
{
public:
    Intersection(double t, const Object& object) :
        m_t(t),
        m_object(object) {}

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

#endif // INTERSECTION_H
