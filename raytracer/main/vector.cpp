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

// sqrt(x^2 + y^2 + z^2 + w^2)
double Vector::magnitude() const
{
    return sqrt( (m_x*m_x) + (m_y*m_y) + (m_z*m_z));
}
