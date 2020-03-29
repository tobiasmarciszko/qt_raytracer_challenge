#ifndef VECTOR_H
#define VECTOR_H

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

    [[nodiscard]] constexpr float magnitude() const {
        // aka length
        // Pythagoras to the rescue!
        return std::sqrt((x*x) + (y*y) + (z*z));
    }

    [[nodiscard]] Vector normalize() const {
        const float magnitude = this->magnitude();
        return {x / magnitude,
                y / magnitude,
                z / magnitude};
    }

    [[nodiscard]] constexpr float dot(const Vector& b) const {
        return x * b.x +
            y * b.y +
            z * b.z;
    }

    [[nodiscard]] Vector cross(const Vector& b) const {
        return {y * b.z - z * b.y,
                z * b.x - x * b.z,
                x * b.y - y * b.x};
    }

    [[nodiscard]] Vector reflect(const Vector& normal) const {
        return {*this - (normal * 2 * dot(normal))};
    }

};

#endif // VECTOR_H
