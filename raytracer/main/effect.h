#ifndef EFFECT_H
#define EFFECT_H

#include "QImage"
typedef QImage FrameBuffer;

#include <QObject>
class IEffect : public QObject
{
    Q_OBJECT

public slots:
    virtual void update() = 0;

public:
    QImage framebuffer;
};

#endif // EFFECT_H
