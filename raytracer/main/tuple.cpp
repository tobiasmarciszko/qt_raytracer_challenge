#include "tuple.h"
#include "point.h"
#include "vector.h"
#include "equal.h"

Tuple::Tuple(float x_, float y_, float z_, float w_) :
    x(x_),
    y(y_),
    z(z_),
    w(w_)
{
}

bool Tuple::isPoint() const {
    return equal(w, 1.0);
}

bool Tuple::isVector() const {
    return equal(w, 0.0);
}

Tuple operator-(const Tuple& t) {
    return {-t.x, -t.y, -t.z, -t.w};
}

Tuple operator+(const Tuple& lhs, const Tuple& rhs) {
    const float newx = lhs.x + rhs.x;
    const float newy = lhs.y + rhs.y;
    const float newz = lhs.z + rhs.z;
    const float neww = lhs.w + rhs.w;

    if (equal(neww, 0)) {
        return Vector(newx, newy, newz);
    }

    return Point(newx, newy, newz);
}

Tuple operator-(const Tuple& lhs, const Tuple& rhs) {
    return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w};
}

Tuple operator*(const Tuple& t, const float m) {
    return {t.x * m, t.y * m, t.z * m, t.w * m};
}

Tuple operator/(const Tuple& t, const float d) {
    return {t.x / d, t.y / d, t.z / d, t.w / d};
}

bool operator==(const Tuple& lhs, const Tuple& rhs) {
    return equal(lhs.x, rhs.x) &&
           equal(lhs.y, rhs.y) &&
           equal(lhs.z, rhs.z) &&
           equal(lhs.w, rhs.w);
}
