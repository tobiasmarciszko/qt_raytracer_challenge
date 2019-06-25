#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include "tuple.h"
#include "point.h"

class Vector : public Tuple
{
public:
    Vector(double x,
           double y,
           double z) :
        Tuple(x, y, z, 0)
    {}

    Vector(const Tuple& tuple) : Tuple(tuple) {}

    inline double magnitude() const
    {
        // aka length
        // Pythagoras to the rescue!
        return std::sqrt( (x*x) + (y*y) + (z*z));
    }

    inline Vector normalize() const
    {
        double magnitude = this->magnitude();
        return {x / magnitude,
                y / magnitude,
                z / magnitude};
    }

    inline double dot(const Vector& b) const
    {
        return (
            x * b.x +
            y * b.y +
            z * b.z);
    }

    inline Vector cross(const Vector& b) const
    {
        return {y * b.z - z * b.y,
                z * b.x - x * b.z,
                x * b.y - y * b.x};
    }

    inline Vector reflect(const Vector& normal) const {
        return *this - (normal * 2 * dot(normal));
    }
};

#endif // VECTOR_H
