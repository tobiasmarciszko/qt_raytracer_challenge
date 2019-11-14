#ifndef SPHERE_H
#define SPHERE_H

#include "shape.h"
#include "materials.h"
#include <vector>

struct Vector;

struct Sphere : public Shape
{
    Vector local_normal_at(const Point& local_point) const override;
    std::vector<Intersection> local_intersect(const Ray& ray) const override;
};

inline Sphere glass_sphere() {
    Sphere s;
    s.set_transform(identity_matrix);
    s.material = Materials::glass;
    return s;
}

#endif // SPHERE_H
