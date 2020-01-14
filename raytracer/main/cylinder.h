#ifndef CYLINDER_H
#define CYLINDER_H

#include "shape.h"
#include <vector>

class Vector;

struct Cylinder : public Shape
{
  [[nodiscard]] Vector local_normal_at(const Point& local_point) const override;
  [[nodiscard]] std::vector<Intersection> local_intersect(const Ray& ray) const override;

  float minimum{-INFINITY};
  float maximum{INFINITY};
};

#endif // CYLINDER_H
