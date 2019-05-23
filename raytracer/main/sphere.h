#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include <cmath>
#include "ray.h"
#include "point.h"
#include "vector.h"
#include "object.h"
#include "matrix.h"

class Intersection;

class Sphere : public Object
{
public:
    Sphere() = default;

    inline void set_transform(const Matrix<4,4>& transform) {
        m_transform = transform;
    }

    inline auto transform() const {
        return m_transform;
    }

    inline Vector normal_at(const Point& world_point) const {
        const Point object_point = m_transform.inverse() * world_point;
        const Vector object_normal = object_point - Point(0, 0, 0);
        Vector world_normal = m_transform.inverse().transpose() * object_normal;

        // Transpose messes with our w value. Hack it back to 0;
        world_normal.resetWToZero();
        return world_normal.normalize();
    }

private:
    Matrix<4,4> m_transform = identity_matrix;
};

#endif // SPHERE_H
