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
        return std::sqrt( (m_x*m_x) + (m_y*m_y) + (m_z*m_z));
    }

    inline Vector normalize() const
    {
        double magnitude = this->magnitude();
        return {m_x / magnitude,
                m_y / magnitude,
                m_z / magnitude};
    }

    inline double dot(const Vector& b) const
    {
        return (m_x * b.x() +
            m_y * b.y() +
            m_z * b.z());
    }

    inline Vector cross(const Vector& b) const
    {
        return {m_y * b.z() - m_z * b.y(),
                m_z * b.x() - m_x * b.z(),
                m_x * b.y() - m_y * b.x()};
    }

    inline Vector reflect(const Vector& normal) const {
        return *this - (normal * 2 * dot(normal));
    }
};

#endif // VECTOR_H
