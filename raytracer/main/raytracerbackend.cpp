#include "raytracerbackend.h"
#include "appsettings.h"
#include "worlds.h"

#include <QDebug>
#include <QElapsedTimer>
#include <QPainter>
#include <QRgb>

static int counter = 0;

using namespace Raytracer::Engine;

RaytracerBackend::RaytracerBackend(QObject *parent)
    : QObject(parent), m_world(Worlds::cornell_box()),
      m_previewWorld(Worlds::materialPreviewWorld()) {
    connect(&m_futureWatcher, SIGNAL(finished()), this, SLOT(renderFinished()));
    connect(&m_materialPreviewFutureWatcher, SIGNAL(finished()), this,
            SLOT(materialPreviewFinished()));
    connect(&m_futureWatcher, SIGNAL(progressValueChanged(int)), this,
            SLOT(progressValueChanged(int)));

    // Material preview
    m_previewCamera.set_transform(
        view_transform(Point(0, 1.0, -1.5), Point(0, 0, 0), Vector(0, 1, 0)));
}

void RaytracerBackend::progressValueChanged(int value) {
    m_progress = value * 100 / m_canvas.pixels.count();
    emit progressChanged();
}

void RaytracerBackend::setViewportSize(int width, int height) {

    if (width < 0 || height < 0)
        return;

    m_camera = Camera(static_cast<float>(width), static_cast<float>(height), M_PI / 6.0F);
    m_camera.set_transform(view_transform(Point(m_fromX, m_fromY, m_fromZ),
                                          Point(m_toX, m_toY, m_toZ),
                                          Vector(0, 1, 0)));
    m_framebuffer = QImage(width, height, QImage::Format_RGB32);
    m_canvas = Canvas(width, height);
}

void RaytracerBackend::render() {

    m_futureWatcher.cancel();
    m_rendering = true;
    emit renderingChanged();

    bool aaEnabled = AppSettings::get().aaEnabled();

    const auto renderPixel = [&, aaEnabled](Pixel &pixel) {
        const Ray ray1 = ray_for_pixel(m_camera, pixel.x, pixel.y);
        const auto color1 = color_at(m_world, ray1, m_lighting, 5);

        if (aaEnabled) {
            // anti alias attempt
            const float size = 0.33;

            const Ray ray2 =
                ray_for_pixel(m_camera, pixel.x + size, pixel.y + size);
            const Ray ray3 =
                ray_for_pixel(m_camera, pixel.x + size, pixel.y - size);
            const Ray ray4 =
                ray_for_pixel(m_camera, pixel.x - size, pixel.y - size);
            const Ray ray5 =
                ray_for_pixel(m_camera, pixel.x - size, pixel.y + size);

            const auto color2 = color_at(m_world, ray2, m_lighting, 5);
            const auto color3 = color_at(m_world, ray3, m_lighting, 5);
            const auto color4 = color_at(m_world, ray4, m_lighting, 5);
            const auto color5 = color_at(m_world, ray5, m_lighting, 5);

            pixel.color = (color1 + color2 + color3 + color4 + color5) / 5;
        } else {
            pixel.color = color1;
        }
    };

    m_timer.start();
    m_futureWatcher.setFuture(QtConcurrent::map(m_canvas.pixels, renderPixel));
}

void RaytracerBackend::materialPreview() {

    m_materialPreviewFutureWatcher.cancel();

    const auto id = m_selectedObject.getId();

    const auto material = m_world.getShapePtrFromId(id)->material;
    m_previewWorld.shapes.at(0)->set_material(material);

    const auto renderPixel = [&](Pixel &pixel) {
        const Ray ray1 = ray_for_pixel(m_previewCamera, pixel.x, pixel.y);
        const auto color1 = color_at(m_previewWorld, ray1, m_lighting, 5);

        // anti alias attempt
        const float size = 0.33;

        const Ray ray2 =
            ray_for_pixel(m_previewCamera, pixel.x + size, pixel.y + size);
        const Ray ray3 =
            ray_for_pixel(m_previewCamera, pixel.x + size, pixel.y - size);
        const Ray ray4 =
            ray_for_pixel(m_previewCamera, pixel.x - size, pixel.y - size);
        const Ray ray5 =
            ray_for_pixel(m_previewCamera, pixel.x - size, pixel.y + size);

        const auto color2 = color_at(m_previewWorld, ray2, m_lighting, 5);
        const auto color3 = color_at(m_previewWorld, ray3, m_lighting, 5);
        const auto color4 = color_at(m_previewWorld, ray4, m_lighting, 5);
        const auto color5 = color_at(m_previewWorld, ray5, m_lighting, 5);

        pixel.color = (color1 + color2 + color3 + color4 + color5) / 5;
    };

    m_materialPreviewFutureWatcher.setFuture(
        QtConcurrent::map(m_previewCanvas.pixels, renderPixel));
}

void RaytracerBackend::wireframe(QImage &framebuffer, const Camera &camera) const {

    QPainter p(&framebuffer);
    p.setRenderHints(QPainter::Antialiasing);
    p.fillRect(QRectF{0, 0, camera.hsize, camera.vsize}, QColor{100,100,100});

    for (const auto& shape : m_world.shapes) {
        const auto m = shape->transform();
        const auto c = shape->material.color;

        QColor qc;
        uint color;
        {
            const auto r = c.red < 1.0F ? c.red : 1.0F;
            const auto g = c.green < 1.0F ? c.green : 1.0F;
            const auto b = c.blue < 1.0F ? c.blue : 1.0F;
            qc.setRgbF(r, g, b);
            color = qRgb(qc.red(), qc.green(), qc.blue());

            if (m_selectedObject.getId() == shape->id) {
                color = qRgb(0, 255, 255);
            }
        }

        const auto centerPoint = m * Point(0, 0, 0);

        const auto xScale = m.get(0, 0);
        const auto yScale = m.get(1, 1);
        const auto zScale = m.get(2, 2);

        if (dynamic_cast<Cube *>(shape.get())) {

            const auto top1 = m * Point(1, 1, -1);  // centerPoint.x + xScale;
            const auto top2 = m * Point(1, 1, 1);   // centerPoint.x - xScale;
            const auto top3 = m * Point(-1, 1, -1); // centerPoint.y + yScale;
            const auto top4 = m * Point(-1, 1, 1);  // centerPoint.y - yScale;

            const auto bot1 = m * Point(1, -1, -1);  // centerPoint.x + xScale;
            const auto bot2 = m * Point(1, -1, 1);   // centerPoint.x - xScale;
            const auto bot3 = m * Point(-1, -1, -1); // centerPoint.y + yScale;
            const auto bot4 = m * Point(-1, -1, 1);  // centerPoint.y - yScale;

            // Top points
            const auto a1 = convertWorldToScreenPoint(camera, top1);
            const auto a2 = convertWorldToScreenPoint(camera, top2);
            const auto a3 = convertWorldToScreenPoint(camera, top3);
            const auto a4 = convertWorldToScreenPoint(camera, top4);

            // Bottom points
            const auto b1 = convertWorldToScreenPoint(camera, bot1);
            const auto b2 = convertWorldToScreenPoint(camera, bot2);
            const auto b3 = convertWorldToScreenPoint(camera, bot3);
            const auto b4 = convertWorldToScreenPoint(camera, bot4);

            p.setPen("black");
            qc.setAlpha(125);
            p.setBrush(QBrush(qc));

            const QPointF top[4] = {a1, a2, a4, a3};
            const QPointF bottom[4] = {b1, b2, b4, b3};
            const QPointF left[4] = {a1, a2, b2, b1};
            const QPointF right[4] = {a3, a4, b4, b3};
            const QPointF front[4] = {a1, a3, b3, b1};
            const QPointF back[4] = {a2, a4, b4, b2};

            for(const auto& point: top) {
                if (point.x() <= -1 || point.y() <= -1)
                    return;
            }

            for(const auto& point: bottom) {
                if (point.x() <= -1 || point.y() <= -1)
                    return;
            }

            p.drawPolygon(top, 4);
            p.drawPolygon(bottom, 4);
            p.drawPolygon(left, 4);
            p.drawPolygon(right, 4);
            p.drawPolygon(front, 4);
            p.drawPolygon(back, 4);
        }

        if (const auto cyl = dynamic_cast<Cylinder *>(shape.get())) {

            const auto top1 = m * Point(0, cyl->maximum, -1);
            const auto top2 = m * Point(0, cyl->maximum, 1);
            const auto top3 = m * Point(1, cyl->maximum, 0);
            const auto top4 = m * Point(-1, cyl->maximum, 0);

            const auto top11 = m * Point(0.75, cyl->maximum, -0.75);
            const auto top21 = m * Point(0.75, cyl->maximum, 0.75);
            const auto top31 = m * Point(-0.75, cyl->maximum, -0.75);
            const auto top41 = m * Point(-0.75, cyl->maximum, 0.75);

            const auto bot1 = m * Point(0, cyl->minimum, -1);
            const auto bot2 = m * Point(0, cyl->minimum, 1);
            const auto bot3 = m * Point(1, cyl->minimum, 0);
            const auto bot4 = m * Point(-1, cyl->minimum, 0);

            const auto bot11 = m * Point(0.75, cyl->minimum, -0.75);
            const auto bot21 = m * Point(0.75, cyl->minimum, 0.75);
            const auto bot31 = m * Point(-0.75, cyl->minimum, -0.75);
            const auto bot41 = m * Point(-0.75, cyl->minimum, 0.75);

            // Top points
            const auto a1 = convertWorldToScreenPoint(camera, top1);
            const auto a2 = convertWorldToScreenPoint(camera, top2);
            const auto a3 = convertWorldToScreenPoint(camera, top3);
            const auto a4 = convertWorldToScreenPoint(camera, top4);

            const auto a11 = convertWorldToScreenPoint(camera, top11);
            const auto a21 = convertWorldToScreenPoint(camera, top21);
            const auto a31 = convertWorldToScreenPoint(camera, top31);
            const auto a41 = convertWorldToScreenPoint(camera, top41);

            // Bottom points
            const auto b1 = convertWorldToScreenPoint(camera, bot1);
            const auto b2 = convertWorldToScreenPoint(camera, bot2);
            const auto b3 = convertWorldToScreenPoint(camera, bot3);
            const auto b4 = convertWorldToScreenPoint(camera, bot4);

            const auto b11 = convertWorldToScreenPoint(camera, bot11);
            const auto b21 = convertWorldToScreenPoint(camera, bot21);
            const auto b31 = convertWorldToScreenPoint(camera, bot31);
            const auto b41 = convertWorldToScreenPoint(camera, bot41);

            // Top "circle"
            drawLine(&p, a1, a11, color);
            drawLine(&p, a11, a3, color);
            drawLine(&p, a2, a21, color);
            drawLine(&p, a21, a3, color);
            drawLine(&p, a1, a31, color);
            drawLine(&p, a31, a4, color);
            drawLine(&p, a2, a41, color);
            drawLine(&p, a41, a4, color);

            // Bottom "circle"
            drawLine(&p, b1,  b11, color);
            drawLine(&p, b11, b3, color);
            drawLine(&p, b2,  b21, color);
            drawLine(&p, b21, b3, color);
            drawLine(&p, b1,  b31, color);
            drawLine(&p, b31, b4, color);
            drawLine(&p, b2,  b41, color);
            drawLine(&p, b41, b4, color);

            // Connect top and bottom
            drawLine(&p, a1, b1, color);
            drawLine(&p, a2, b2, color);
            drawLine(&p, a3, b3, color);
            drawLine(&p, a4, b4, color);

            drawLine(&p, a11, b11, color);
            drawLine(&p, a21, b21, color);
            drawLine(&p, a31, b31, color);
            drawLine(&p, a41, b41, color);
        }

        if (dynamic_cast<Sphere *>(shape.get())) {
            p.setPen(color);

            const Point top = m * Point{0, 1, 0};
            const Point bottom = m * Point{0, -1, 0};

            const auto t = convertWorldToScreenPoint(camera, top);
            const auto b = convertWorldToScreenPoint(camera, bottom);

            const Point front = m * Point{0, 0, -1};
            const Point back = m * Point{0, 0, 1};

            const auto fr = convertWorldToScreenPoint(camera, front);
            const auto ba = convertWorldToScreenPoint(camera, back);

            const Point left = m * Point{-1, 0, 0};
            const Point right= m * Point{1, 0, 0};

            const auto l = convertWorldToScreenPoint(camera, left);
            const auto r = convertWorldToScreenPoint(camera, right);

            p.drawLine(t, b);
            p.drawLine(fr, ba);
            p.drawLine(l, r);
        }

        if (dynamic_cast<Plane *>(shape.get())) {

            const auto a2 = convertWorldToScreenPoint(camera, m * Point(1, 0, 0));
            const auto a3 = convertWorldToScreenPoint(camera, m * Point(-1, 0, 0));
            const auto a4 = convertWorldToScreenPoint(camera, m * Point(0, 0, -1));
            const auto a5 = convertWorldToScreenPoint(camera, m * Point(0, 0, 1));

            drawLine(&p, a2, a3, color);
            drawLine(&p, a4, a5, color);

            const auto b2 = convertWorldToScreenPoint(camera, m * Point(10, 0, 0));
            const auto b3 = convertWorldToScreenPoint(camera, m * Point(-10, 0, 0));
            const auto b4 = convertWorldToScreenPoint(camera, m * Point(0, 0, -10));
            const auto b5 = convertWorldToScreenPoint(camera, m * Point(0, 0, 10));

            drawLine(&p, b2, b3, color);
            drawLine(&p, b4, b5, color);
        }
    }
}

void RaytracerBackend::wireframe() const {

    Camera front{m_camera.hsize, m_camera.vsize, m_camera.field_of_view};
    Camera left{m_camera.hsize, m_camera.vsize, m_camera.field_of_view};
    Camera right{m_camera.hsize, m_camera.vsize, m_camera.field_of_view};

    front.set_transform(
        view_transform(Point(0, 0, m_fromZ), Point(0, 0, 0), Vector(0, 1, 0)));
    left.set_transform(
        view_transform(Point(-m_fromZ, 0, 0), Point(0, 0, 0), Vector(0, 1, 0)));
    right.set_transform(
        view_transform(Point(m_fromZ, 0, 0), Point(0, 0, 0), Vector(0, 1, 0)));

    QImage front_framebuffer{static_cast<int>(front.hsize),
                             static_cast<int>(front.vsize),
                             QImage::Format_RGB32};
    QImage left_framebuffer{static_cast<int>(left.hsize),
                            static_cast<int>(left.vsize), QImage::Format_RGB32};
    QImage right_framebuffer{static_cast<int>(right.hsize),
                             static_cast<int>(right.vsize),
                             QImage::Format_RGB32};
    QImage persp_framebuffer{static_cast<int>(m_camera.hsize),
                             static_cast<int>(m_camera.vsize),
                             QImage::Format_RGB32};

    wireframe(front_framebuffer, front);
    wireframe(left_framebuffer, left);
    wireframe(right_framebuffer, right);
    wireframe(persp_framebuffer, m_camera);

    emit wireframesReady(front_framebuffer, left_framebuffer, right_framebuffer,
                         persp_framebuffer);
}

void RaytracerBackend::renderFinished() {
    m_lastRenderTime = static_cast<int>(m_timer.elapsed());
    m_timer.start();

    copyFrameBuffer(m_canvas, m_framebuffer);

    m_rendering = false;
    emit renderingChanged();
    emit imageReady(m_framebuffer);
    QString filename = "render" + QString::number(counter) + ".png";
    m_framebuffer.save(filename, "PNG", 100);
}

void RaytracerBackend::materialPreviewFinished() {
    // Pointer to first pixel
    QRgb *data = reinterpret_cast<QRgb *>(m_previewframebuffer.bits());
    QColor color;

    for (const Pixel &pixel : m_previewCanvas.pixels) {
        const auto &red = pixel.color.red;
        const auto &green = pixel.color.green;
        const auto &blue = pixel.color.blue;

        const auto &r = red < 1.0 ? red : 1.0;
        const auto &g = green < 1.0 ? green : 1.0;
        const auto &b = blue < 1.0 ? blue : 1.0;

        color.setRgbF(r, g, b);

        *data = color.rgb();
        data++;
    }
    emit materialPreviewReady(m_previewframebuffer);
}

void RaytracerBackend::copyFrameBuffer(Canvas &from, QImage &to) {
    // Pointer to first pixel
    QRgb *data = reinterpret_cast<QRgb *>(to.bits());
    QColor color;

    for (const Pixel &pixel : from.pixels) {
        const auto &red = pixel.color.red;
        const auto &green = pixel.color.green;
        const auto &blue = pixel.color.blue;

        const auto &r = red < 1.0 ? red : 1.0;
        const auto &g = green < 1.0 ? green : 1.0;
        const auto &b = blue < 1.0 ? blue : 1.0;

        color.setRgbF(r, g, b);

        *data = color.rgb();
        data++;
    }
}

// Convert world coordinate to screen space
// Returns the screen coordinates as a Point (ignore z)
QPointF RaytracerBackend::convertWorldToScreenPoint(const Camera &camera,
                                                  const Point &point) const {

    // Transform point in world coordinate to camera
    const auto pCamera = camera.transform() * point;

    // Ignore points behind the camera
    if (pCamera.z >= 0)
        return {-1, -1};

    // Perspective divide
    const auto pScreenx = pCamera.x / pCamera.z;
    const auto pScreeny = pCamera.y / -pCamera.z;

    // Scale and offset acordingly according to the screen size
    const auto p1xoffset = camera.half_width + pScreenx;
    const auto p1yoffset = camera.half_height - pScreeny;
    const auto pRasterx = ((p1xoffset / camera.pixel_size) - 0.5);
    const auto pRastery = ((p1yoffset / camera.pixel_size) - 0.5);

    // Enable to draw pixel instead of just converting the coordinates
#if 0
  // Clip before draw to avoid plotting outside the screen
        if (pRasterx >=0 &&
            pRastery >=0 &&
            pRasterx < m_camera.hsize &&
            pRastery < m_camera.vsize) {
            setPixel(pRasterx, pRastery, color);
        }
#endif

    // Return the pixel coordinates on the canvas/screen
    return {pRasterx, pRastery};
}

void RaytracerBackend::setPixel(QImage &framebuffer, int x, int y, uint color) {

    // Raw access to pixel, I don't know if this is equivalent to using
    // QPainter's "drawPoint"
#if 0
  QRgb *pixel = reinterpret_cast<QRgb *>(m_framebuffer.scanLine(y)); // select row
        pixel += x; // select column
        *pixel = color;
#endif
    QPainter p(&framebuffer);
    p.setPen(color);
    p.drawPoint(x, y);
}

void RaytracerBackend::drawLine(QImage &framebuffer, const QPointF &p1,
                                const QPointF &p2, uint color) const {
    // drawLine(p1.x, p1.y, p2.x, p2.y, color);

    if (p1.x() == -1 && p1.y() == -1)
        return;
    if (p2.x() == -1 && p2.y() == -1)
        return;

    QPainter p(&framebuffer);
    p.setRenderHints(QPainter::Antialiasing);
    p.setPen(color);
    p.drawLine(p1, p2);
}


void RaytracerBackend::drawLine(QPainter* p, const QPointF &p1,
                                const QPointF &p2, uint color) const {

    if (p1.x() == -1 && p1.y() == -1)
        return;
    if (p2.x() == -1 && p2.y() == -1)
        return;

    p->setPen(color);
    p->drawLine(p1, p2);
}

void RaytracerBackend::switchChanged() {
    if (m_lighting == LightingModel::Phong) {
        m_lighting = LightingModel::BlinnPhong;

    } else if (m_lighting == LightingModel::BlinnPhong) {
        m_lighting = LightingModel::Phong;
    }
}

void RaytracerBackend::translate(float x, float y, float z) {
    const bool prepend = true;
    appendTransform(translation(x, y, z), prepend);
}

void RaytracerBackend::scale(float x, float y, float z) {
    appendTransform(scaling(x, y, z));
}

void RaytracerBackend::rotate_x(float angle) {
    appendTransform(rotation_x(angle));
}

void RaytracerBackend::rotate_y(float angle) {
    appendTransform(rotation_y(angle));
}

void RaytracerBackend::rotate_z(float angle) {
    appendTransform(rotation_z(angle));
}

void RaytracerBackend::appendTransform(const Matrix<4, 4> &t,
                                       bool prepend) {

    if (m_selectedObject.getId() == -1) return;

    auto shape_ptr = m_world.getShapePtrFromId(m_selectedObject.getId());
    auto transform = shape_ptr->transform();

    if (prepend) {
        transform = t * transform;
    } else {
        transform = transform * t;
    }

    shape_ptr->set_transform(transform);
    m_selectedObject.setShapePointer(shape_ptr);
    emit selectedObjectChanged();
}
