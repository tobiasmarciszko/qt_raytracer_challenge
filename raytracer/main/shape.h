#ifndef SHAPE_H
#define SHAPE_H

#include <cstdlib>
#include <vector>
#include <memory>
#include "material.h"
#include "point.h"
#include "vector.h"
#include "matrix.h"
#include "ray.h"
#include "intersection.h"

static int shape_count{0};

struct Shape
{
    virtual ~Shape() = default;
    Shape() = default;

    bool operator==(const Shape& object) const {
        return object.id == id;
    }

    inline void set_material(const Material& m) {
        material = m;
    }

    inline void set_transform(const Matrix<4,4>& transform) {
        m_transform = transform;
        m_inverse_transform = m_transform.inverse();
    }

    [[nodiscard]] inline auto transform() const {
        return m_transform;
    }

    [[nodiscard]] inline auto inverse_transform() const {
        return m_inverse_transform;
    }

    [[nodiscard]] virtual Vector normal_at(const Point& world_point) const {
        const Point local_point = m_inverse_transform * world_point;
        const Vector local_normal = local_normal_at(local_point);
        Vector world_normal = m_inverse_transform.transpose() * local_normal;
        world_normal.w = 0;
        return world_normal.normalize();
    }

    [[nodiscard]] virtual Vector local_normal_at(const Point& world_point) const = 0;

    [[nodiscard]] std::vector<Intersection> intersect(const Ray& r) const {
        const auto local_ray = r.transform(m_inverse_transform);
        return local_intersect(local_ray);
    }

    [[nodiscard]] virtual std::vector<Intersection> local_intersect(const Ray& r) const = 0;


    const int id{shape_count++};
    Material material;

private:
    Matrix<4,4> m_transform{identity_matrix};
    Matrix<4,4> m_inverse_transform{identity_matrix};
};

inline Color pattern_at_shape(const std::shared_ptr<Pattern>& pattern_ptr, const Shape* shape, const Point& world_point) {
    const auto object_point = shape->inverse_transform() * world_point;
    const auto pattern_point = pattern_ptr->inverse_transform * object_point;

    return pattern_ptr->pattern_at(pattern_point);
}

#endif // SHAPE_H
