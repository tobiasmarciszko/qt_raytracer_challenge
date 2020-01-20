#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include "tuple.h"

struct Vector : public Tuple
{
    Vector() = default;
    Vector(float x,
           float y,
           float z) :
        Tuple(x, y, z, 0)
    {}

    Vector(const Tuple& tuple) : Tuple(tuple) {}

    [[nodiscard]] float magnitude() const;
    [[nodiscard]] Vector normalize() const;
    [[nodiscard]] float dot(const Vector& b) const;
    [[nodiscard]] Vector cross(const Vector& b) const;
    [[nodiscard]] Vector reflect(const Vector& normal) const;
};

#endif // VECTOR_H
