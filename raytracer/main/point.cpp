#include "point.h"
#include <QDebug>

Point::Point(double x,
             double y,
             double z) :
    Tuple(x, y, z, 1)
{
    qDebug() << "Created Point. x:" << x << " y:" << y << " z:" << z;
}
