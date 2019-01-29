#include "tuple.h"
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

Tuple Tuple::Point(qreal x, qreal y, qreal z) {
    return Tuple(x, y, z, 1);
}

Tuple Tuple::Vector(qreal x, qreal y, qreal z) {
    return Tuple(x, y, z, 0);
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


