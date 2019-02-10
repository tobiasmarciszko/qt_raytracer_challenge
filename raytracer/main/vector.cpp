#include "vector.h"
#include <QDebug>
#include <cmath>

Vector::Vector(double x,
               double y,
               double z) :
    Tuple(x, y, z, 0)
{
    qDebug() << "Created Vector. x:" << x << " y:" << y << " z:" << z;
}

double Vector::magnitude() const
{
    // aka length
    // Pythagoras to the rescue!
    return sqrt( (m_x*m_x) + (m_y*m_y) + (m_z*m_z));
}

Vector Vector::normalize() const
{
    double magnitude = this->magnitude();
    return Vector(m_x / magnitude,
                  m_y / magnitude,
                  m_z / magnitude);
}
