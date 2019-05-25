#ifndef COMPUTATIONS_H
#define COMPUTATIONS_H

#include "sphere.h"
#include "point.h"
#include "vector.h"
#include "intersection.h"
#include "ray.h"

struct Computations {

    Computations() = default;

    double t = -1;
    Sphere object = Sphere();
    Point point = Point(0,0,0);
    Vector eyev = Vector(0,0,0);
    Vector normalv = Vector(0,0,0);
    bool inside = false;
};

Computations prepare_computations(const Intersection& i, const Ray& r) {
    Computations comps;

    comps.t = i.t();
    comps.object = i.object();

    comps.point = r.position(comps.t);
    comps.eyev = -r.direction();
    comps.normalv = comps.object.normal_at(comps.point);

    if (comps.normalv.dot(comps.eyev) < 0) {
       comps.inside = true;
       comps.normalv = -comps.normalv;
    } else {
        comps.inside = false;
    }

    return comps;
}

#endif //COMPUTATIONS_H
