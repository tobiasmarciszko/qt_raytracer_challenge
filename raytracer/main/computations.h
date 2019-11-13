#ifndef COMPUTATIONS_H
#define COMPUTATIONS_H

#include "sphere.h"
#include "point.h"
#include "vector.h"
#include "intersection.h"
#include "ray.h"
#include <memory>

struct Computations {
    float t{-1};
    const Shape* object;
    Point point{0,0,0};
    Vector eyev{0,0,0};
    Vector normalv{0,0,0};
    Vector reflectv{0,0,0};
    Point over_point{point + normalv * EPSILON};
    Point under_point{point - normalv * EPSILON};
    bool inside{false};
    float n1{0};
    float n2{0};
};

inline Computations prepare_computations(const Intersection& i, const Ray& r, const Intersections& xs = {}) {
    Computations comps;

    comps.t = i.t;
    comps.object = i.object;
    comps.point = r.position(comps.t);
    comps.eyev = -r.direction();
    comps.normalv = comps.object->normal_at(comps.point);

    if (comps.normalv.dot(comps.eyev) < 0) {
        comps.inside = true;
        comps.normalv = -comps.normalv;
    } else {
        comps.inside = false;
    }

    comps.reflectv = r.direction().reflect(comps.normalv);

    comps.over_point = comps.point + comps.normalv * EPSILON;
    comps.under_point = comps.point - comps.normalv * EPSILON;

    // Refraction
    std::vector<const Shape*> containers{};

    for(const auto& is: xs) {
        if (i == is) {
            if (containers.empty()) {
                comps.n1 = 1.0;
            } else {
                comps.n1 = containers.back()->material.refractive_index;
            }
        }

        bool found = false;
        for (auto it = containers.begin(); it != containers.end(); it++) {
            if (is.object == *it) {
                containers.erase(it);
                found = true;
                break;
            }
        }

        if (!found) {
            containers.push_back(is.object);
        }

        if (i == is) {
            if (containers.empty()) {
                comps.n2 = 1.0;
            } else {
                comps.n2 = containers.back()->material.refractive_index;
            }

            break;
        }
    }

    return comps;
}

#endif //COMPUTATIONS_H
