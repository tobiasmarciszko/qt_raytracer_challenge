#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include <cmath>
#include "ray.h"
#include "point.h"
#include "vector.h"
#include "object.h"
#include "matrix.h"

class Sphere : public Object
{
public:
    Sphere() = default;

    inline Vector normal_at(const Point& world_point) const override {
        const Point object_point = m_transform.inverse() * world_point;
        const Vector object_normal = object_point - Point(0, 0, 0);
        Vector world_normal = m_transform.inverse().transpose() * object_normal;

        // Transpose messes with our w value. Hack it back to 0;
        world_normal.resetWToZero();
        return world_normal.normalize();
    }
};

#endif // SPHERE_H
