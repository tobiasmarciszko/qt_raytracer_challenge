#ifndef CUBE_H
#define CUBE_H

#include "shape.h"
#include <vector>

struct Vector;

struct Cube : public Shape
{
    Vector local_normal_at(const Point& local_point) const override;
    std::vector<Intersection> local_intersect(const Ray& ray) const override;
};

#endif // CUBE_H
