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
#include "engine.h"

class RaytracerBackend : public QObject
{
    Q_OBJECT

public:
    RaytracerBackend(QObject *parent = nullptr);

    ShapeQmlBridge* getSelectedObject() {
        return &m_selectedObject;
    }

public:
    Q_PROPERTY(int progress MEMBER m_progress NOTIFY progressChanged)
    Q_PROPERTY(bool rendering MEMBER m_rendering NOTIFY renderingChanged)
    Q_PROPERTY(double fromX MEMBER m_fromX NOTIFY fromXChanged)
    Q_PROPERTY(double fromY MEMBER m_fromY NOTIFY fromYChanged)
    Q_PROPERTY(double fromZ MEMBER m_fromZ NOTIFY fromZChanged)
    Q_PROPERTY(double lastRenderTime MEMBER m_lastRenderTime NOTIFY imageReady)
    Q_PROPERTY(ShapeQmlBridge* selectedObject READ getSelectedObject NOTIFY selectedObjectChanged)

public slots:
    void render();
    void wireframe();
    void materialPreview();
    void switchChanged();
    void setViewportSize(int width, int height);

    void selectObject(unsigned int x, unsigned int y ) {
        const Ray ray = ray_for_pixel(m_camera, x, y);
        const auto is = Raytracer::Engine::intersect_world(m_world, ray);
        const auto h = Raytracer::Engine::hit(is);
        if (h.has_value()) {
            auto shape_ptr = m_world.getShapePtrFromId(h.value().object->id);
            m_selectedObject.setShapePointer(shape_ptr);
            wireframe();
            materialPreview();

            emit selectedObjectChanged();
        }
    }

    void translate(unsigned int id, float x, float y, float z);
    void scale(unsigned int id, float x, float y, float z);

signals:
    void imageReady(const QImage& image);
    void wireframesReady(const QImage& top, const QImage& left, const QImage& right, const QImage& perspective);
    void materialPreviewReady(const QImage& image);
    void renderingChanged();
    void progressChanged();
    void fromXChanged();
    void fromYChanged();
    void fromZChanged();
    void selectedObjectChanged();

private slots:
    void renderFinished();
    void materialPreviewFinished();
    void progressValueChanged(int value);

private: // Variables

    // Camera
    double m_fromX = 1;
    double m_fromY = 1;
    double m_fromZ = -4;

    double m_toX = 0;
    double m_toY = 1;
    double m_toZ = 0;

    // For rendering
    Canvas m_canvas;
    Camera m_camera;
    QImage m_framebuffer;

    World m_world;
    World m_previewWorld;

    LightingModel m_lighting = LightingModel::Phong;
    bool m_rendering = false;
    int m_progress{0};
    int m_lastRenderTime{0};
    ShapeQmlBridge m_selectedObject{this};

    QFutureWatcher<void> m_futureWatcher;
    QFutureWatcher<void> m_materialPreviewfutureWatcher;
    QElapsedTimer m_timer;

    // Material preview
    Canvas m_previewCanvas{140, 140};
    Camera m_previewCamera{140, 140, M_PI / 3.0};
    QImage m_previewframebuffer{140, 140, QImage::Format_RGB32};

private: // Methods
    void drawLine(QImage& framebuffer, int x0, int y0, int x1, int y1, uint color = qRgb(255, 255, 255));
    void drawLine(QImage& framebuffer, const Point& p1, const Point& p2, uint color = qRgb(255, 255, 255));
    Point convertWorldToScreenPoint(const Camera& camera, const Point& point);
    void setPixel(QImage& framebuffer, int x, int y, uint color = qRgb(255, 255, 255));
    void copyFrameBuffer(Canvas& from, QImage& to);
    void wireframe(QImage& framebuffer, const Camera& camera);
};

#endif // RAYTRACER_H
