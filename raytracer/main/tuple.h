#ifndef TUPLE_H
#define TUPLE_H

#include <iostream>
#include "equal.h"

struct Tuple
{
    Tuple() = default;
    Tuple(float x_, float y_, float z_, float w_) :
        x(x_),
        y(y_),
        z(z_),
        w(w_)
    {
    }

    [[nodiscard]] friend Tuple operator-(const Tuple& t) {
        return {-t.x, -t.y, -t.z, -t.w};
    }

    [[nodiscard]] friend Tuple operator+(const Tuple& lhs, const Tuple& rhs) {
        const float newx = lhs.x + rhs.x;
        const float newy = lhs.y + rhs.y;
        const float newz = lhs.z + rhs.z;
        const float neww = lhs.w + rhs.w;

        return {newx, newy, newz, neww};
    }

    [[nodiscard]] friend Tuple operator-(const Tuple& lhs, const Tuple& rhs) {
        return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w};
    }

    [[nodiscard]] friend Tuple operator*(const Tuple& t, float m) {
        return {t.x * m, t.y * m, t.z * m, t.w * m};
    }

    [[nodiscard]] friend Tuple operator/(const Tuple& t, float d) {
        return {t.x / d, t.y / d, t.z / d, t.w / d};
    }

    [[nodiscard]] friend bool operator==(const Tuple& lhs, const Tuple& rhs) {
        return equal(lhs.x, rhs.x) &&
            equal(lhs.y, rhs.y) &&
            equal(lhs.z, rhs.z) &&
            equal(lhs.w, rhs.w);
    }

    [[nodiscard]] bool isPoint() const {
        return equal(w, 1.0);
    }

    [[nodiscard]] bool isVector() const {
        return equal(w, 0.0);
    }

    friend std::ostream &operator<<(std::ostream &os, const Tuple& t) {
        return os << (t.isVector() ? "Vector" : "Point") << "(" << t.x << ", " << t.y << ", " << t.z << ")";
    }

    float x{0};
    float y{0};
    float z{0};
    float w{0};
};

#endif // TUPLE_H
