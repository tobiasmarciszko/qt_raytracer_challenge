#include "vector.h"
#include <QDebug>

Vector::Vector(double x,
               double y,
               double z) :
    Tuple(x, y, z, 0)
{
    qDebug() << "Created Point. x:" << x << " y:" << y << " z:" << z;
}
