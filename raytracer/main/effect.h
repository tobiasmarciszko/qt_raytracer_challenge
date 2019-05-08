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

public:
    FrameBuffer framebuffer;
};

#endif // EFFECT_H
