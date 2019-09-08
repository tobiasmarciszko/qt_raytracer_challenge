#ifndef POINT_H
#define POINT_H

#include "tuple.h"

struct Point : public Tuple
{
    Point(float x, float y, float z);
    Point(const Tuple& tuple) : Tuple(tuple) {}
};

#endif // POINT_H
