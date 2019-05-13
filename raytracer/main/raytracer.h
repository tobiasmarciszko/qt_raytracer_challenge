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
    void update() override;

private:
    void projectileEffect();
    void clockEffect();

    // Boilerplate
private:
    void copyCanvasToFramebuffer();

private:
        Canvas<320,240> m_canvas;
};

#endif // RAYTRACER_H
