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

  bool closed{false};

private:
  [[nodiscard]] bool check_cap(const Ray& ray, float t) const;
  void intersect_caps(const Ray& ray, std::vector<Intersection>& xs) const;

};

#endif // CYLINDER_H
