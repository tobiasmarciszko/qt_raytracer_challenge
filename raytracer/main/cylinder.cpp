#include "cylinder.h"
#include "intersection.h"
#include "vector.h"
#include <algorithm>
#include <cmath>

Vector Cylinder::local_normal_at(const Point& local_point) const {

  const auto absx = std::abs(local_point.x);
  const auto absy = std::abs(local_point.y);
  const auto absz = std::abs(local_point.z);

  const auto maxc = std::max({absx, absy, absz});

  if (equal(maxc, absx)) {
    return Vector{local_point.x, 0, 0};
  }

  if (equal(maxc, absy)) {
    return Vector{0, local_point.y, 0};
  }

  return Vector{0, 0, local_point.z};
}

std::vector<Intersection> Cylinder::local_intersect(const Ray& ray) const {

  const float a = (ray.direction().x * ray.direction().x) + (ray.direction().z * ray.direction().z);

  // Ray is parallel to the y axis (a is apx 0)
  if (equal(a, 0)) return {};

  const float b = (2 * ray.origin().x * ray.direction().x) +
                  (2 * ray.origin().z * ray.direction().z);
  const float c = (ray.origin().x * ray.origin().x) +
                  (ray.origin().z * ray.origin().z) - 1.0F;

  const float discriminant = (b * b) - (4.0F * a * c);

  // No hit
  if (discriminant < 0) return {};

  const float t0 = (-b - std::sqrt(discriminant)) / ( 2.0F * a);
  const float t1 = (-b + std::sqrt(discriminant)) / ( 2.0F * a);

  // Hit
  return {Intersection(t0, this), Intersection(t1, this)};
}
