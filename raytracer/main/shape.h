#ifndef OBJECT_H
#define OBJECT_H

#include <cstdlib>
#include <vector>
#include "material.h"
#include "point.h"
#include "vector.h"
#include "matrix.h"
#include "ray.h"

class Intersection;
class Shape
{
public:

    virtual ~Shape() = default;

    inline int id() const {
        return m_id;
    }

    inline Material material() const {
        return m_material;
    }

    inline void set_material(const Material& m) {
        m_material = m;
    }

    bool operator==(const Shape& object) const {
        return object.id() == m_id;
    }

    inline void set_transform(const Matrix<4,4>& transform) {
        m_transform = transform;
    }

    inline auto transform() const {
        return m_transform;
    }

    virtual Vector normal_at(const Point& world_point) const = 0;
    virtual std::vector<Intersection> intersect(const Ray& r) const = 0;

protected:
    Matrix<4,4> m_transform = identity_matrix;

private:
    int m_id = std::rand();
    Material m_material = Material();
};

#endif // OBJECT_H
