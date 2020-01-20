#ifndef TUPLE_H
#define TUPLE_H

#include <iostream>

struct Tuple
{
    Tuple() = default;
    Tuple(float x, float y, float z, float w);

    friend Tuple operator-(const Tuple& t);
    friend Tuple operator+(const Tuple& lhs, const Tuple& rhs);
    friend Tuple operator-(const Tuple& lhs, const Tuple& rhs);
    friend Tuple operator*(const Tuple& t, float multiplier);
    friend Tuple operator/(const Tuple& t, float divider);
    friend bool operator==(const Tuple& lhs, const Tuple& rhs);

    [[nodiscard]] bool isPoint() const;
    [[nodiscard]] bool isVector() const;

    friend std::ostream &operator<<(std::ostream &os, const Tuple& t) {
        return os << (t.isVector() ? "Vector" : "Point") << "(" << t.x << ", " << t.y << ", " << t.z << ")";
    }

    float x{0};
    float y{0};
    float z{0};
    float w{0};
};

#endif // TUPLE_H
