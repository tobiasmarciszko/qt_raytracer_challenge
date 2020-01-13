#ifndef CUBE_H
#define CUBE_H

#include "shape.h"
#include <vector>

class Vector;

struct Cylinder : public Shape
{
  [[nodiscard]] Vector local_normal_at(const Point& local_point) const override;
  [[nodiscard]] std::vector<Intersection> local_intersect(const Ray& ray) const override;
};

#endif // CUBE_H
