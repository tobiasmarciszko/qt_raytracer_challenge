#ifndef POINT_H
#define POINT_H

#include "tuple.h"

struct Point : public Tuple
{
    Point(float x, float y, float z, float w = 1) : Tuple(x, y, z, w) {}
    Point(const Tuple& tuple) : Tuple(tuple) {} //NOLINT (Implicit conversion used)
};

#endif // POINT_H
