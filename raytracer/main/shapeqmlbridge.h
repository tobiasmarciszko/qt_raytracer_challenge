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
    Q_PROPERTY(QColor color READ getColor WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QMatrix4x4 transform READ getTransform)
    Q_PROPERTY(uint id READ getId)
    Q_PROPERTY(float ambient READ getAmbient WRITE setAmbient NOTIFY ambientChanged)
    Q_PROPERTY(float diffuse READ getDiffuse WRITE setDiffuse NOTIFY diffuseChanged)
    Q_PROPERTY(float specular READ getSpecular WRITE setSpecular NOTIFY specularChanged)

public:
    explicit ShapeQmlBridge(QObject *parent = nullptr);
    void setShapePointer(std::shared_ptr<Shape> ptr) {
        m_shape_ptr = ptr;
    }

    void setColor(QColor color) {
        m_shape_ptr->material.color = Color(color.redF(), color.greenF(), color.blueF());
        emit colorChanged();
    }

    void setAmbient(float value) {
        m_shape_ptr->material.ambient = value;
        emit ambientChanged();
    }

    void setDiffuse(float value) {
        m_shape_ptr->material.diffuse = value;
        emit diffuseChanged();
    }

    void setSpecular(float value) {
        m_shape_ptr->material.specular = value;
        emit specularChanged();
    }

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

    float getAmbient() const {
        return m_shape_ptr->material.ambient;
    }

    float getDiffuse() const {
        return m_shape_ptr->material.diffuse;
    }

    float getSpecular() const {
        return m_shape_ptr->material.specular;
    }

signals:
    void colorChanged();
    void ambientChanged();
    void diffuseChanged();
    void specularChanged();

private:
    std::shared_ptr<Shape> m_shape_ptr{nullptr};


};

#endif // SHAPEQMLBRIDGE_H
