#include "tuple.h"
#include "point.h"
#include "vector.h"

#include <QDebug>
#include <cmath>

bool equal(const double& a, const double& b) {
    return std::abs(a - b) < EPSILON;
}

Tuple::Tuple(double x_, double y_, double z_, double w_) :
    x(x_),
    y(y_),
    z(z_),
    w(w_)
{
    // qDebug() << "Created Tuple. x:" << x << " y:" << y << " z:" << z << " w:" << w;
}

bool Tuple::isPoint() const {
    return equal(w, 1.0);
}

bool Tuple::isVector() const {
    return equal(w, 0.0);
}

Tuple Tuple::operator-() const {
    return {-x, -y, -z, -w};
}

Tuple Tuple::operator+(const Tuple& tuple) const {
    const double newx = x + tuple.x;
    const double newy = y + tuple.y;
    const double newz = z + tuple.z;
    const double neww = w + tuple.w;

    if (equal(neww, 0)) {
        return Vector(newx, newy, newz);
    }

    return Point(newx, newy, newz);
}

Tuple Tuple::operator-(const Tuple& t) const {
    return {x - t.x, y - t.y, z - t.z, w -  t.w};
}

Tuple Tuple::operator*(const double& m) const {
    return {x * m, y * m, z * m, w * m};
}

Tuple Tuple::operator/(const double& d) const {
    return {x / d, y / d, z / d, w / d};
}

bool Tuple::operator==(const Tuple& tuple) const {
    if (!equal(x, tuple.x)) return false;
    if (!equal(y, tuple.y)) return false;
    if (!equal(z, tuple.z)) return false;
    if (!equal(w, tuple.w)) return false;

    return true;
}
