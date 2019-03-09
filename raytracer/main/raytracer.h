#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <QObject>
#include "effect.h"
#include "canvas.h"

class raytracer : public IEffect
{
public:
    raytracer();

public slots:
    virtual void update();

private:
    Canvas m_canvas;
};

#endif // RAYTRACER_H
