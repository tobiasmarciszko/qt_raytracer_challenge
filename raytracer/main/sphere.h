#ifndef SPHERE_H
#define SPHERE_H

#include "shape.h"
#include <vector>

struct Vector;

struct Sphere : public Shape
{
    Vector local_normal_at(const Point& local_point) const override;
    std::vector<Intersection> local_intersect(const Ray& ray) const override;
};

#endif // SPHERE_H
