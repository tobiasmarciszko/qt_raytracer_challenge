#include "tuple.h"
#include "point.h"
#include "vector.h"

#include <QDebug>
#include <cmath>

bool equal(double a, double b) {
    if (std::abs(a -b) < EPSILON) {
    return true;
} else {
    return false;
    }
}

Tuple::Tuple(qreal x, qreal y, qreal z, qreal w) :
    m_x(x),
    m_y(y),
    m_z(z),
    m_w(w)
{
    qDebug() << "Created Tuple. x:" << x << " y:" << y << " z:" << z << " w:" << w;
}

bool Tuple::isPoint() {
    return equal(m_w, 1.0);
}

bool Tuple::isVector() {
    return equal(m_w, 0.0);
}

Tuple Tuple::operator-() {
    return Tuple(-m_x, -m_y, -m_z, -m_w);
}

Tuple Tuple::operator+(const Tuple& tuple) const {
    double x = m_x + tuple.m_x;
    double y = m_y + tuple.m_y;
    double z = m_z + tuple.m_z;
    double w = m_w + tuple.m_w;
    return Tuple(x, y, z, w);
}

Tuple Tuple::operator-(const Tuple& tuple) const {
    double x = m_x - tuple.m_x;
    double y = m_y - tuple.m_y;
    double z = m_z - tuple.m_z;
    double w = m_w - tuple.m_w;
    return Tuple(x, y, z, w);
}

Tuple Tuple::operator*(const double& multiplier) const {
    double x = m_x * multiplier;
    double y = m_y * multiplier;
    double z = m_z * multiplier;
    double w = m_w * multiplier;
    return Tuple(x, y, z, w);
}

Tuple Tuple::operator/(const double& divider) const {
    double x = m_x / divider;
    double y = m_y / divider;
    double z = m_z / divider;
    double w = m_w / divider;
    return Tuple(x, y, z, w);
}
