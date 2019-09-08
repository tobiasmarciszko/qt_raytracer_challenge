#include "point.h"
#include <QDebug>

Point::Point(float x,
             float y,
             float z) :
    Tuple(x, y, z, 1)
{
    // qDebug() << "Created Point. x:" << x << " y:" << y << " z:" << z;
}
