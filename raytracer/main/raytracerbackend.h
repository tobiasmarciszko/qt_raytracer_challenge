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
    explicit RaytracerBackend(QObject *parent = nullptr);

    ShapeQmlBridge* getSelectedObject() {
        return &m_selectedObject;
    }

    Q_PROPERTY(int progress MEMBER m_progress NOTIFY progressChanged)
    Q_PROPERTY(bool rendering MEMBER m_rendering NOTIFY renderingChanged)
    Q_PROPERTY(float fromX MEMBER m_fromX NOTIFY fromXChanged)
    Q_PROPERTY(float fromY MEMBER m_fromY NOTIFY fromYChanged)
    Q_PROPERTY(float fromZ MEMBER m_fromZ NOTIFY fromZChanged)
    Q_PROPERTY(float lastRenderTime MEMBER m_lastRenderTime NOTIFY imageReady)
    Q_PROPERTY(ShapeQmlBridge* selectedObject READ getSelectedObject NOTIFY selectedObjectChanged)

public slots:
    void render();
    void wireframe();
    void materialPreview();
    void switchChanged();
    void setViewportSize(float width, float height);

    void selectObject(float x, float y) {
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

    void translate(float x, float y, float z);
    void scale(float x, float y, float z);
    void rotate_x(float angle);
    void rotate_y(float angle);
    void rotate_z(float angle);
// end slots

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
    void linesReady(const QList<QLine>& lines);

private slots:
    void renderFinished();
    void materialPreviewFinished();
    void progressValueChanged(int value);
    void frameTick();

private:
    void drawLine(QImage& framebuffer, const Point& p1, const Point& p2, uint color = qRgb(255, 255, 255));
    Point convertWorldToScreenPoint(const Camera& camera, const Point& point);
    void setPixel(QImage& framebuffer, int x, int y, uint color = qRgb(255, 255, 255));
    void copyFrameBuffer(Canvas& from, QImage& to);
    void wireframe(QImage& framebuffer, const Camera& camera);
    void appendTransform(const Matrix<4,4>& transform, bool prepend = false);

    // Camera
    float m_fromX = 1;
    float m_fromY = 1;
    float m_fromZ = -5;

    float m_toX = 0;
    float m_toY = 0.5;
    float m_toZ = 0;

    // For rendering
    Canvas m_canvas;

    // Cameras
    Camera m_camera;
    QImage m_framebuffer;

    Camera m_frontCamera;
    Camera m_leftCamera;
    Camera m_rightCamera;

    // Perspectives
    QImage m_front_framebuffer{static_cast<int>(m_frontCamera.hsize),
                               static_cast<int>(m_frontCamera.vsize),
                               QImage::Format_RGB32};
    QImage m_left_framebuffer{static_cast<int>(m_leftCamera.hsize),
                              static_cast<int>(m_leftCamera.vsize),
                               QImage::Format_RGB32};
    QImage m_right_framebuffer{static_cast<int>(m_rightCamera.hsize),
                               static_cast<int>(m_rightCamera.vsize),
                               QImage::Format_RGB32};
    QImage m_persp_framebuffer{static_cast<int>(m_camera.hsize),
                               static_cast<int>(m_camera.vsize),
                               QImage::Format_RGB32};
    World m_world;
    World m_previewWorld;

    LightingModel m_lighting = LightingModel::Phong;
    bool m_rendering = false;
    int m_progress{0};
    int m_lastRenderTime{0};
    ShapeQmlBridge m_selectedObject{this};

    QFutureWatcher<void> m_futureWatcher;
    QFutureWatcher<void> m_materialPreviewFutureWatcher;
    QElapsedTimer m_timer;
    QElapsedTimer m_wireframeTimer;

    // Material preview
    Canvas m_previewCanvas{140, 140};
    Camera m_previewCamera{140, 140, M_PI / 3.0};
    QImage m_previewframebuffer{140, 140, QImage::Format_RGB32};

    QTimer m_intervalTimer;
    Matrix<4,4> m_cubeRotation = rotation_x(0.0174533) * rotation_z(0.0174533);
};

#endif // RAYTRACER_H
