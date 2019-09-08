#ifndef SHAPEQMLBRIDGE_H
#define SHAPEQMLBRIDGE_H

#include <QObject>

class ShapeQmlBridge : public QObject
{
    Q_OBJECT
public:
    explicit ShapeQmlBridge(QObject *parent = nullptr);

signals:

public slots:
};

#endif // SHAPEQMLBRIDGE_H
