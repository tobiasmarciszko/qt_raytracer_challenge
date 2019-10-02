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
#include "shapeqmlbridge.h"

class Raytracer : public QObject
{
    Q_OBJECT

public:
    explicit Raytracer(QObject *parent = nullptr);

public:
    Q_PROPERTY(int progress MEMBER m_progress NOTIFY progressChanged)
    Q_PROPERTY(bool rendering MEMBER m_rendering NOTIFY renderingChanged)
    Q_PROPERTY(double fromX MEMBER m_fromX NOTIFY fromXChanged)
    Q_PROPERTY(double fromY MEMBER m_fromY NOTIFY fromYChanged)
    Q_PROPERTY(double fromZ MEMBER m_fromZ NOTIFY fromZChanged)
    Q_PROPERTY(double lastRenderTime MEMBER m_lastRenderTime NOTIFY imageReady)

public slots:
    void render();
    void wireframe();
    void materialPreview();
    void setViewportSize(int width, int height);
    void renderFinished();
    void materialPreviewFinished();
    void progressValueChanged(int value);
    void switchChanged();

    QObject* objectFromCoordinates(unsigned int x, unsigned int y) {
        const Ray ray = ray_for_pixel(m_camera, x, y);
        const auto is = intersect_world(m_world, ray);
        const auto h = hit(is);
        if (!h.has_value()) {
            return nullptr;
        }

        m_currentShapeBridge = createShapeQmlBridge(h.value().object);
        m_selectedMaterial = h.value().object->material();
        return m_currentShapeBridge;
    }

signals:
    void imageReady(const QImage& image);
    void materialPreviewReady(const QImage& image);
    void renderingChanged();
    void progressChanged();
    void fromXChanged();
    void fromYChanged();
    void fromZChanged();

private:
    // Camera
    double m_fromX = 0;
    double m_fromY = 1.5;
    double m_fromZ = -4;

    double m_toX = 0;
    double m_toY = 1;
    double m_toZ = 0;

    // Viewport
    int m_width = 640;
    int m_height = 480;

    Canvas m_canvas = Canvas(640, 480);
    Camera m_camera = Camera(640, 480, M_PI / 3.0);
    QImage m_framebuffer = QImage(640, 480, QImage::Format_RGB32);

    World m_world;
    World m_previewWorld;

    LightingModel m_lighting = LightingModel::Phong;
    bool m_rendering = false;
    int m_progress{0};
    int m_lastRenderTime{0};
    ShapeQmlBridge* m_currentShapeBridge = nullptr;

    QFutureWatcher<void> m_futureWatcher;
    QFutureWatcher<void> m_materialPreviewfutureWatcher;
    QElapsedTimer m_timer;

    void drawLine(int x1, int y1, int x2, int y2, uint color = qRgb(255, 255, 255));
    void drawLine(const Point& p1, const Point& p2, uint color = qRgb(255, 255, 255));
    Point drawWorldPoint(const Point& point, uint color = qRgb(255, 255, 255));
    void setPixel(int x, int y, uint color = qRgb(255, 255, 255));
    void copyFrameBuffer();

    // Material preview
    Canvas m_previewCanvas = Canvas(140, 140);
    Camera m_previewCamera = Camera(140, 140, M_PI / 3.0);
    QImage m_previewframebuffer = QImage(140, 140, QImage::Format_RGB32);
    Material m_selectedMaterial;
};

#endif // RAYTRACER_H
