#ifndef RAYTRACER_H
#define RAYTRACER_H

// #include <QObject>
#include "effect.h"
#include "canvas.h"

class Camera;
class World;

class raytracer : public IEffect
{
public:
    raytracer();

public slots:
    void update() override;

    // Effects
private:
    void projectileEffect();
    void clockEffect();
    void flatSphere();
    void shadedSphere();
    void defaultWorld();

    // Helpers
private:
        void render(const Camera& camera, const World& world);
        void writePixel(int x, int y, const Color& color);

private:
        Canvas<320,240> m_canvas;
};

#endif // RAYTRACER_H
