#include "vector.h"
#include <QDebug>
#include <cmath>

Vector::Vector(double x,
               double y,
               double z) :
    Tuple(x, y, z, 0)
{
    // qDebug() << "Created Vector. x:" << x << " y:" << y << " z:" << z;
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
    return {m_x / magnitude,
                  m_y / magnitude,
                  m_z / magnitude};
}

double Vector::dot(const Vector& b) const
{
    return (m_x * b.x() +
            m_y * b.y() +
            m_z * b.z());
}

Vector Vector::cross(const Vector& b) const
{
    return {m_y * b.z() - m_z * b.y(),
                  m_z * b.x() - m_x * b.z(),
                  m_x * b.y() - m_y * b.x()};
}
