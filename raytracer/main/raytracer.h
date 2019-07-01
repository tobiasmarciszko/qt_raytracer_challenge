#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <QObject>
#include <QImage>
#include <QFuture>
#include <QFutureWatcher>
#include <QtCore/QFutureSynchronizer>
#include <QtConcurrent>
#include "canvas.h"
#include "camera.h"
#include "world.h"

using FrameBuffer = QImage;

class Raytracer : public QObject
{
    Q_OBJECT

public:

    Q_PROPERTY(int progress READ getProgress NOTIFY progressChanged)
    int getProgress() {
        return m_futureWatcher.progressValue() * 100 / m_canvas.m_pixels.size();
    }

    Q_PROPERTY(bool rendering READ getRendering WRITE setRendering NOTIFY renderingChanged)
    bool getRendering() {
        return m_rendering;
    }

    void setRendering(bool rendering) {
        m_rendering = rendering;
        emit renderingChanged();
    }

public:
    explicit Raytracer(QObject *parent = nullptr);

public slots:
    void update();
    void render(int width, int height);
    void renderFinished();
    void progressValueChanged(int value);

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
    void renderingChanged();
    void progressChanged();

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
    void render();

private:
    Canvas m_canvas = Canvas(320, 240);
    Camera m_camera = Camera(320, 240, M_PI / 3);
    World m_world = default_world();
    LightingModel m_lighting = LightingModel::Phong;
    bool m_rendering = false;

    QFutureWatcher<void> m_futureWatcher;
    QElapsedTimer m_timer;
};

#endif // RAYTRACER_H
