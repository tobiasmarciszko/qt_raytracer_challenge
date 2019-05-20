#ifndef RAY_H
#define RAY_H

#include "point.h"
#include "vector.h"
#include "matrix.h"

class Ray
{
public:
    Ray(const Point& origin, const Vector& direction) :
        m_origin(origin),
        m_direction(direction) {}

    inline Point origin() const {
        return m_origin;
    }

    inline Vector direction() const {
        return m_direction;
    }

    inline Point position(const double t) const {
        return m_origin + m_direction * t;
    }

    inline Ray transform(const Matrix<4,4>& matrix) const {
        return {matrix * m_origin, matrix * m_direction};
    }

private:
    Point m_origin;
    Vector m_direction;
};

#endif // RAY_H
