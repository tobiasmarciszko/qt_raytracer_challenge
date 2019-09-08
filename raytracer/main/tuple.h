#ifndef TUPLE_H
#define TUPLE_H

#include <iostream>

#define EPSILON 0.00001
bool equal(const float& a, const float& b);

struct Tuple
{
    Tuple(float x, float y, float z, float w);

    Tuple operator -() const;
    Tuple operator +(const Tuple& tuple) const;
    Tuple operator -(const Tuple& tuple) const;
    Tuple operator *(const float& multiplier) const;
    Tuple operator /(const float& divider) const;
    bool operator ==(const Tuple& tuple) const;

    bool isPoint() const;
    bool isVector() const;

    friend std::ostream &operator<<(std::ostream &os, const Tuple& t) {
        return os << (t.isVector() ? "Vector" : "Point") << "(" << t.x << ", " << t.y << ", " << t.z << ")";
    }

    float x;
    float y;
    float z;
    float w;
};

#endif // TUPLE_H
