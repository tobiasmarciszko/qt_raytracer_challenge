#include "tuple.h"
#include <QDebug>
#include <cmath>

Tuple::Tuple(QObject *parent) : QObject(parent)
{

}

Tuple::Tuple(qreal x, qreal y, qreal z, qreal w) :
    m_x(x),
    m_y(y),
    m_z(z),
    m_w(w)
{
    qDebug() << "Created Tuple. x:" << x << " y:" << y << " z:" << z << " w:" << w;
}

Tuple* Tuple::Point(qreal x, qreal y, qreal z) {
    return new Tuple(x, y, z, 1);
}

Tuple* Tuple::Vector(qreal x, qreal y, qreal z) {
    return new Tuple(x, y, z, 0);
}

bool Tuple::Equal(qreal a, qreal b) {
    if (std::abs(a -b) < 0.00001) {
        return true;
    } else {
        return false;
    }
}

bool Tuple::isPoint() {
    return Tuple::Equal(m_w, 1.0);
}

bool Tuple::isVector() {
    return Tuple::Equal(m_w, 0.0);
}
