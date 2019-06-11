#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <QObject>
#include <QImage>
#include "canvas.h"

class Camera;
class World;

using FrameBuffer = QImage;

class Raytracer : public QObject
{
    Q_OBJECT

public:
    explicit Raytracer(QObject *parent = nullptr);

public slots:
    void update();
    void fromXChanged(double val) { fromX = val; update(); }
    void fromYChanged(double val) { fromY = val; update(); }

signals:
    void rendererReady(const QImage& image);

public:
    FrameBuffer framebuffer = QImage(320, 240, QImage::Format_RGB32);

public:
    double fromX = 0;
    double fromY = 1.5;
    double fromZ = -5;

    double toX = 0;
    double toY = 1;
    double toZ = 0;

    // Effects
private:
    void projectileEffect();
    void clockEffect();
    void flatSphere();
    void shadedSphere();
    void defaultWorld();
    void threeBallsOnAFloor();

    // Helpers
private:
        void render(const Camera& camera, const World& world);
        void writePixel(int x, int y, const Color& color);

private:
        Canvas<320,240> m_canvas = Canvas<320,240>();
};

#endif // RAYTRACER_H
