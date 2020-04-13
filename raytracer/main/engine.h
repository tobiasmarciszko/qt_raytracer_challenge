#ifndef ENGINE_H
#define ENGINE_H

#include "matrix.h"
#include <vector>
#include <optional>

struct Computations;
struct Intersection;
struct Ray;
struct World;
struct Point;
struct Light;
struct Color;
struct Material;
struct Shape;
struct Vector;

enum class LightingModel {
    Phong,
    BlinnPhong
};

using Intersections = std::vector<Intersection>;

namespace Raytracer::Engine {
    Computations prepare_computations(const Intersection& i, const Ray& r, const Intersections& xs = {});
    Intersections intersect_world(const World& w, const Ray& r);
    [[nodiscard]] bool is_shadowed(const World& world, const Point& light_position, const Point& point);
    Color shade_hit(const World& w, const Computations& comps, const LightingModel& lightingModel = LightingModel::Phong, int remaining = 5);
    Color refracted_color(const World& world, const Computations& comps, const LightingModel& lightingModel = LightingModel::Phong, int remaining = 4);
    float schlick(const Computations& comps);
    Color lighting(const Material& material, const Shape* object, const Light& light, const Point& point, const Vector& eyev, const Vector& normalv, float intensity = 1.0, const LightingModel& lightingModel = LightingModel::Phong);
    Color color_at(const World& w, const Ray& r, const LightingModel& lightingModel = LightingModel::Phong, int remaining = 4);
    Color reflected_color(const World& w, const Computations& comps, const LightingModel& lightingModel = LightingModel::Phong, int remaining = 4);
    Color reflected_color(const World& w, const Computations& comps, int remaining);
    Matrix<4,4> view_transform(const Point& from, const Point& to, const Vector& up);
    std::optional<Intersection> hit(Intersections intersections);
    [[nodiscard]] float intensity_at(const Light& light, const Point& point, const World& world);
}

#endif // ENGINE_H
