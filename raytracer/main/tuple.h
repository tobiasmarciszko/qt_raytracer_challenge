#ifndef TUPLE_H
#define TUPLE_H

#include <iostream>

#define EPSILON 0.00001
bool equal(const double& a, const double& b);

struct Tuple
{
    Tuple(double x, double y, double z, double w);

    Tuple operator -() const;
    Tuple operator +(const Tuple& tuple) const;
    Tuple operator -(const Tuple& tuple) const;
    Tuple operator *(const double& multiplier) const;
    Tuple operator /(const double& divider) const;
    bool operator ==(const Tuple& tuple) const;

    bool isPoint() const;
    bool isVector() const;

    friend std::ostream &operator<<(std::ostream &os, const Tuple& t) {
        return os << (t.isVector() ? "Vector" : "Point") << "(" << t.x << ", " << t.y << ", " << t.z << ")";
    }

    double x;
    double y;
    double z;
    double w;
};

#endif // TUPLE_H
