#ifndef SHAPEQMLBRIDGE_H
#define SHAPEQMLBRIDGE_H

#include <QObject>
#include <QColor>
#include <QMatrix4x4>
#include "color.h"
#include "shape.h"
#include "intersection.h"

class ShapeQmlBridge : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QColor color MEMBER color)
    Q_PROPERTY(QMatrix4x4 transform MEMBER transform)
    Q_PROPERTY(uint id MEMBER id)

public:
    explicit ShapeQmlBridge(QObject *parent = nullptr);

public:
    QMatrix4x4 transform;
    QColor color;
    uint id;
};

inline void createShapeQmlBridge(ShapeQmlBridge& bridge, const Shape* shape_ptr) {

    QColor qcolor;
    const QMatrix4x4 matrix(shape_ptr->transform().get(0,0),
                      shape_ptr->transform().get(0,1),
                      shape_ptr->transform().get(0,2),
                      shape_ptr->transform().get(0,3),
                      shape_ptr->transform().get(1,0),
                      shape_ptr->transform().get(1,1),
                      shape_ptr->transform().get(1,2),
                      shape_ptr->transform().get(1,3),
                      shape_ptr->transform().get(2,0),
                      shape_ptr->transform().get(2,1),
                      shape_ptr->transform().get(2,2),
                      shape_ptr->transform().get(2,3),
                      shape_ptr->transform().get(3,0),
                      shape_ptr->transform().get(3,1),
                      shape_ptr->transform().get(3,2),
                      shape_ptr->transform().get(3,3));

    Color color = shape_ptr->material.color;
    qcolor.setRgbF(color.red, color.green, color.blue);

    bridge.color = qcolor;
    bridge.transform = matrix;
    bridge.id = shape_ptr->id;
}
#endif // SHAPEQMLBRIDGE_H
