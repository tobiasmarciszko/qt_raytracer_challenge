#ifndef EFFECT_H
#define EFFECT_H

#include "QImage"
using FrameBuffer = QImage;

#include <QObject>
class IEffect : public QObject
{
    Q_OBJECT

public slots:
    virtual void update() = 0;

signals:
    void rendererReady();

public:
    FrameBuffer framebuffer;

public:
    double fromX = 0;
    double fromY = 1.5;
    double fromZ = -5;

    double toX = 0;
    double toY = 1;
    double toZ = 0;

};

#endif // EFFECT_H
