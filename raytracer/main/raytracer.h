#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <QObject>
#include <QImage>
#include "canvas.h"
#include "camera.h"
#include "world.h"

using FrameBuffer = QImage;

class Raytracer : public QObject
{
    Q_OBJECT

public:
    explicit Raytracer(QObject *parent = nullptr);

public slots:
    void update();

    void fromXChanged(double val) {
        fromX = val;
        m_camera.transform = view_transform(Point(fromX, fromY, fromZ), Point(toX, toY, toZ), Vector(0, 1, 0));
        update();
    }

    void fromYChanged(double val) {
        fromY = val;
        m_camera.transform = view_transform(Point(fromX, fromY, fromZ), Point(toX, toY, toZ), Vector(0, 1, 0));
        update();
    }

    void fromXYChanged(double x, double y) {
        fromX += x;
        fromY += y;
        m_camera.transform = view_transform(Point(fromX, fromY, fromZ), Point(toX, toY, toZ), Vector(0, 1, 0));
        update();
    }

    void switchChanged();

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
    void render(const Camera& camera, const World& world, unsigned int fromX, unsigned int toX, unsigned int fromY, unsigned int toY);

    void renderTopL(const Camera& camera, const World& world);
    void renderTopR(const Camera& camera, const World& world);
    void renderBottomL(const Camera& camera, const World& world);
    void renderBottomR(const Camera& camera, const World& world);

    void writePixel(unsigned int x, unsigned int y, const Color& color);

private:
    Canvas<320,240> m_canvas = Canvas<320,240>();
    Camera m_camera = Camera(320, 240, M_PI / 3);
    World m_world = default_world();
    LightingModel m_lighting = LightingModel::Phong;

};

#endif // RAYTRACER_H
