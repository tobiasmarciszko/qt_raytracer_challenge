#ifndef COMPUTATIONS_H
#define COMPUTATIONS_H

#include "shape.h"
#include "point.h"
#include "vector.h"

struct Computations {
    float t{-1};
    const Shape* object{nullptr};
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

#endif //COMPUTATIONS_H
