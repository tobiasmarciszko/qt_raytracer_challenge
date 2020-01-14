#include "cylinder.h"
#include "intersection.h"
#include "vector.h"
#include <algorithm>
#include <cmath>

Vector Cylinder::local_normal_at(const Point& local_point) const {

  const auto dist = local_point.x * local_point.x + local_point.z * local_point.z;

  if (dist < 1 && (local_point.y > maximum - EPSILON || equal(local_point.y, maximum))) {
    return Vector{0, 1, 0};
  }

  if (dist < 1 && (local_point.y < minimum - EPSILON || equal(local_point.y, minimum))) {
    return Vector{0, -1, 0};
  }

  return Vector{local_point.x, 0, local_point.z};
}

std::vector<Intersection> Cylinder::local_intersect(const Ray& ray) const {

  std::vector<Intersection> xs{};

  const float a = (ray.direction().x * ray.direction().x) + (ray.direction().z * ray.direction().z);

  // Ray is parallel to the y axis (a is apx 0)
  if (equal(a, 0)) {
    intersect_caps(ray, xs);
    return xs;
  }

  const float b = (2 * ray.origin().x * ray.direction().x) +
                  (2 * ray.origin().z * ray.direction().z);
  const float c = (ray.origin().x * ray.origin().x) +
                  (ray.origin().z * ray.origin().z) - 1.0F;

  const float discriminant = (b * b) - (4.0F * a * c);

  // No hit
  if (discriminant < 0) return {};

  float t0 = (-b - std::sqrt(discriminant)) / ( 2.0F * a);
  float t1 = (-b + std::sqrt(discriminant)) / ( 2.0F * a);

  if (t0 > t1) std::swap(t0, t1);

  const auto y0 = ray.origin().y + t0 * ray.direction().y;
  if (minimum < y0 && y0 < maximum) {
    xs.emplace_back(Intersection(t0, this));
  }

  const auto y1 = ray.origin().y + t1 * ray.direction().y;
  if (minimum < y1 && y1 < maximum) {
    xs.emplace_back(Intersection(t1, this));
  }

  intersect_caps(ray, xs);

  return xs;
}
bool Cylinder::check_cap(const Ray& ray, float t) const {
  const auto x = ray.origin().x + t * ray.direction().x;
  const auto z = ray.origin().z + t * ray.direction().z;

  const auto radius = (x * x + z * z);

  return (radius < 1 || equal(radius, 1));
}
void Cylinder::intersect_caps(const Ray& ray,
                              std::vector<Intersection>& xs) const {

  if (!closed || equal(ray.direction().y, 0)) return;

  auto t = (minimum - ray.origin().y) / ray.direction().y;
  if (check_cap(ray, t)) {
    xs.emplace_back(Intersection(t, this));
  }

  t = (maximum - ray.origin().y) / ray.direction().y;
  if (check_cap(ray, t)) {
    xs.emplace_back(Intersection(t, this));
  }
}
