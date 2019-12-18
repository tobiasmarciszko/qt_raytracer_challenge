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
    Q_PROPERTY(QColor color READ getColor)
    Q_PROPERTY(QMatrix4x4 transform READ getTransform)
    Q_PROPERTY(uint id READ getId)

public:
    explicit ShapeQmlBridge(QObject *parent = nullptr);
    void setShapePointer(std::shared_ptr<Shape> ptr) {
        m_shape_ptr = ptr;
    }

private:
    std::shared_ptr<Shape> m_shape_ptr{nullptr};

    uint getId() const {
        return m_shape_ptr->id;
    }

    QColor getColor() const {
        QColor qcolor;
        Color color = m_shape_ptr->material.color;
        qcolor.setRgbF(color.red, color.green, color.blue);
        return qcolor;
    }

    QMatrix4x4 getTransform() const {
        return QMatrix4x4(m_shape_ptr->transform().get(0,0),
                          m_shape_ptr->transform().get(0,1),
                          m_shape_ptr->transform().get(0,2),
                          m_shape_ptr->transform().get(0,3),
                          m_shape_ptr->transform().get(1,0),
                          m_shape_ptr->transform().get(1,1),
                          m_shape_ptr->transform().get(1,2),
                          m_shape_ptr->transform().get(1,3),
                          m_shape_ptr->transform().get(2,0),
                          m_shape_ptr->transform().get(2,1),
                          m_shape_ptr->transform().get(2,2),
                          m_shape_ptr->transform().get(2,3),
                          m_shape_ptr->transform().get(3,0),
                          m_shape_ptr->transform().get(3,1),
                          m_shape_ptr->transform().get(3,2),
                          m_shape_ptr->transform().get(3,3));
    }
};

//inline void createShapeQmlBridge(ShapeQmlBridge& bridge, std::shared_ptr<Shape> ptr) {

//    QColor qcolor;
//    const QMatrix4x4 matrix(shape_ptr->transform().get(0,0),
//                      shape_ptr->transform().get(0,1),
//                      shape_ptr->transform().get(0,2),
//                      shape_ptr->transform().get(0,3),
//                      shape_ptr->transform().get(1,0),
//                      shape_ptr->transform().get(1,1),
//                      shape_ptr->transform().get(1,2),
//                      shape_ptr->transform().get(1,3),
//                      shape_ptr->transform().get(2,0),
//                      shape_ptr->transform().get(2,1),
//                      shape_ptr->transform().get(2,2),
//                      shape_ptr->transform().get(2,3),
//                      shape_ptr->transform().get(3,0),
//                      shape_ptr->transform().get(3,1),
//                      shape_ptr->transform().get(3,2),
//                      shape_ptr->transform().get(3,3));

//    Color color = shape_ptr->material.color;
//    qcolor.setRgbF(color.red, color.green, color.blue);

//    bridge.color = qcolor;
//    bridge.transform = matrix;
//    bridge.id = shape_ptr->id;
//}
#endif // SHAPEQMLBRIDGE_H
