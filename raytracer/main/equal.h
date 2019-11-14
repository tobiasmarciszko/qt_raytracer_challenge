#ifndef EQUAL_H
#define EQUAL_H

#include <cmath>

constexpr auto EPSILON = 0.0001f;

inline bool equal(float x, float y)
{
    return std::fabs(x-y) < EPSILON;
}

#endif // EQUAL_H
