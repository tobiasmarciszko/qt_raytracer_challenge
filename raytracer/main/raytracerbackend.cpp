#include "raytracerbackend.h"
#include "worlds.h"

#include <QRgb>
#include <QDebug>
#include <QElapsedTimer>
#include <QPainter>

static int counter = 0;

using namespace Raytracer::Engine;

RaytracerBackend::RaytracerBackend(QObject *parent) :
    QObject(parent),
    m_world(Worlds::cubes()),
    m_previewWorld(Worlds::materialPreviewWorld())
{
    connect(&m_futureWatcher, SIGNAL(finished()), this, SLOT(renderFinished()));
    connect(&m_materialPreviewfutureWatcher, SIGNAL(finished()), this, SLOT(materialPreviewFinished()));
    connect(&m_futureWatcher, SIGNAL(progressValueChanged(int)), this, SLOT(progressValueChanged(int)));

    // Material preview
    m_previewCamera.set_transform(view_transform(Point(0, 1.0, -1.5), Point(0, 0, 0), Vector(0, 1, 0)));
}

void RaytracerBackend::progressValueChanged(int value)
{
    m_progress = value * 100 / m_canvas.pixels.count();
    emit progressChanged();
}

void RaytracerBackend::setViewportSize(int width, int height) {

    if (width < 0 || height < 0) return;

    m_camera = Camera(width, height, M_PI / 3.0);
    m_camera.set_transform(view_transform(Point(m_fromX, m_fromY, m_fromZ), Point(m_toX, m_toY, m_toZ), Vector(0, 1, 0)));
    m_framebuffer = QImage(width, height, QImage::Format_RGB32);
    m_canvas = Canvas(width, height);
}

void RaytracerBackend::render() {

    m_futureWatcher.cancel();
    m_rendering = true;
    emit renderingChanged();

    const auto renderPixel = [&](Pixel& pixel) {
        const Ray ray = ray_for_pixel(m_camera, pixel.x, pixel.y);
        pixel.color = color_at(m_world, ray, m_lighting, 5);
    };

    m_timer.start();
    m_futureWatcher.setFuture(QtConcurrent::map(m_canvas.pixels, renderPixel));
}

void RaytracerBackend::materialPreview() {

    m_materialPreviewfutureWatcher.cancel();
    m_previewWorld.shapes.at(0)->set_material(m_selectedMaterial);

    std::function<void(Pixel&)> renderPixel = [&](Pixel &pixel) {
        const Ray ray = ray_for_pixel(m_previewCamera, pixel.x, pixel.y);
        pixel.color = color_at(m_previewWorld, ray, m_lighting);
    };

    m_materialPreviewfutureWatcher.setFuture(QtConcurrent::map(m_previewCanvas.pixels, renderPixel));
}

void RaytracerBackend::wireframe(QImage& framebuffer, const Camera& camera) {

    framebuffer.fill(QColor(100, 100, 100));

    for (auto& shape: m_world.shapes) {
        auto m = shape->transform();
        auto c = shape->material.color;

        QColor qc;
        const auto r = c.red < 1.0F ? c.red : 1.0F;
        const auto g = c.green < 1.0F ? c.green : 1.0F;
        const auto b = c.blue < 1.0F ? c.blue : 1.0F;
        qc.setRgbF(r, g, b);
        uint color = qRgb(qc.red(), qc.green(), qc.blue());

        const auto centerPoint = m * Point(0, 0, 0);

        const auto xScale = m.get(0, 0);
        const auto yScale = m.get(1, 1);
        const auto zScale = m.get(2, 2);

        const auto top1 = m * Point(1, 1, -1);//centerPoint.x + xScale;
        const auto top2 = m * Point(1, 1, 1); // centerPoint.x - xScale;
        const auto top3 = m * Point(-1, 1, -1);// centerPoint.y + yScale;
        const auto top4 = m * Point(-1, 1, 1);// centerPoint.y - yScale;

        const auto bot1 = m * Point(1, -1, -1);//centerPoint.x + xScale;
        const auto bot2 = m * Point(1, -1, 1); // centerPoint.x - xScale;
        const auto bot3 = m * Point(-1, -1, -1);// centerPoint.y + yScale;
        const auto bot4 = m * Point(-1, -1, 1);// centerPoint.y - yScale;

        if (dynamic_cast<Cube *>(shape.get())) {

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

            // Top square
            drawLine(framebuffer, a1, a2, color);
            drawLine(framebuffer, a3, a4, color);
            drawLine(framebuffer, a1, a3, color);
            drawLine(framebuffer, a2, a4, color);

            // Bottom square
            drawLine(framebuffer, b1, b2, color);
            drawLine(framebuffer, b3, b4, color);
            drawLine(framebuffer, b1, b3, color);
            drawLine(framebuffer, b2, b4, color);

            // Connect top and bottom
            drawLine(framebuffer, a1, b1, color);
            drawLine(framebuffer, a2, b2, color);
            drawLine(framebuffer, a3, b3, color);
            drawLine(framebuffer, a4, b4, color);
        }

        if (dynamic_cast<Sphere *>(shape.get())) {

            QPainter p(&framebuffer);
            p.setRenderHints(QPainter::Antialiasing);
            p.setPen(color);

            Point top{centerPoint.x, centerPoint.y+yScale, centerPoint.z};
            Point bottom{centerPoint.x, centerPoint.y-yScale, centerPoint.z};

            auto t = convertWorldToScreenPoint(camera, top);
            auto b = convertWorldToScreenPoint(camera, bottom);

            Point front{centerPoint.x, centerPoint.y, centerPoint.z-zScale};
            Point back{centerPoint.x, centerPoint.y, centerPoint.z+zScale};

            auto fr = convertWorldToScreenPoint(camera, front);
            auto ba = convertWorldToScreenPoint(camera, back);

            Point left{centerPoint.x-xScale, centerPoint.y, centerPoint.z};
            Point right{centerPoint.x+xScale, centerPoint.y, centerPoint.z};

            auto l = convertWorldToScreenPoint(camera, left);
            auto r = convertWorldToScreenPoint(camera, right);

            p.drawLine(t.x, t.y, b.x, b.y);
            p.drawLine(fr.x, fr.y, ba.x, ba.y);
            p.drawLine(l.x, l.y, r.x, r.y);
        }
    }
}


void RaytracerBackend::wireframe() {

    Camera front{m_camera.hsize, m_camera.vsize, m_camera.field_of_view};
    Camera left{m_camera.hsize, m_camera.vsize, m_camera.field_of_view};
    Camera right{m_camera.hsize, m_camera.vsize, m_camera.field_of_view};

    front.set_transform(view_transform(Point(0, 1, m_fromZ), Point(0, 1, 0), Vector(0, 1, 0)));
    left.set_transform(view_transform(Point(-m_fromZ, 1, 0), Point(0, 1, 0), Vector(0, 1, 0)));
    right.set_transform(view_transform(Point(m_fromZ, 1, 0), Point(0, 1, 0), Vector(0, 1, 0)));
    m_camera.set_transform(view_transform(Point(m_fromX, m_fromY, m_fromZ), Point(m_toX, m_toY, m_toZ), Vector(0, 1, 0)));

    QImage front_framebuffer{static_cast<int>(front.hsize), static_cast<int>(front.vsize), QImage::Format_RGB32};
    QImage left_framebuffer{static_cast<int>(left.hsize), static_cast<int>(left.vsize), QImage::Format_RGB32};
    QImage right_framebuffer{static_cast<int>(right.hsize), static_cast<int>(right.vsize), QImage::Format_RGB32};
    QImage persp_framebuffer{static_cast<int>(m_camera.hsize), static_cast<int>(m_camera.vsize), QImage::Format_RGB32};

    wireframe(front_framebuffer, front);
    wireframe(left_framebuffer, left);
    wireframe(right_framebuffer, right);
    wireframe(persp_framebuffer, m_camera);

    emit wireframesReady(front_framebuffer, left_framebuffer, right_framebuffer, persp_framebuffer);
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

    for (const Pixel& pixel: m_previewCanvas.pixels) {
        const auto& red = pixel.color.red;
        const auto& green = pixel.color.green;
        const auto& blue = pixel.color.blue;

        const auto& r = red < 1.0 ? red : 1.0;
        const auto& g = green < 1.0 ? green : 1.0;
        const auto& b = blue < 1.0 ? blue : 1.0;

        color.setRgbF(r, g, b);

        *data = color.rgb();
        data++;
    }
    emit materialPreviewReady(m_previewframebuffer);
}

void RaytracerBackend::copyFrameBuffer(Canvas& from, QImage& to) {
    // Pointer to first pixel
    QRgb *data = reinterpret_cast<QRgb *>(to.bits());
    QColor color;

    for (const Pixel& pixel: from.pixels) {
        const auto& red = pixel.color.red;
        const auto& green = pixel.color.green;
        const auto& blue = pixel.color.blue;

        const auto& r = red < 1.0 ? red : 1.0;
        const auto& g = green < 1.0 ? green : 1.0;
        const auto& b = blue < 1.0 ? blue : 1.0;

        color.setRgbF(r, g, b);

        *data = color.rgb();
        data++;
    }
}

// Convert world coordinate to screen space
// Returns the screen coordinates as a Point (ignore z)
Point RaytracerBackend::convertWorldToScreenPoint(const Camera& camera, const Point& point, uint color) {

    // Transform point in world coordinate to camera
    auto pCamera = camera.transform() * point;

    // Ignore points behind the camera
    if (pCamera.z > 0) return {-1, -1, -1};

    // Perspective divide
    auto pScreenx = pCamera.x / pCamera.z;
    auto pScreeny = pCamera.y / - pCamera.z;

    // Scale and offset acordingly according to the screen size
    auto p1xoffset = camera.half_width + pScreenx;
    auto p1yoffset = camera.half_height - pScreeny;
    auto pRasterx = ((p1xoffset / camera.pixel_size) - 0.5);
    auto pRastery = ((p1yoffset / camera.pixel_size) - 0.5);

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
    return Point(pRasterx, pRastery, 0);
}

void RaytracerBackend::setPixel(QImage& framebuffer, int x, int y, uint color) {

// Raw access to pixel, I don't know if this is equivalent to using QPainter's "drawPoint"
#if 0
    QRgb *pixel = reinterpret_cast<QRgb *>(m_framebuffer.scanLine(y)); // select row
    pixel += x; // select column
    *pixel = color;
#endif
    QPainter p(&framebuffer);
    p.setPen(color);
    p.drawPoint(x, y);
}

void RaytracerBackend::drawLine(QImage& framebuffer, const Point& p1, const Point& p2, uint color) {
    // drawLine(p1.x, p1.y, p2.x, p2.y, color);

    if (p1.x == -1 && p1.y == -1) return;
    if (p2.x == -1 && p2.y == -1) return;

    QPainter p(&framebuffer);
    p.setRenderHints(QPainter::Antialiasing);
    p.setPen(color);
    p.drawLine(p1.x, p1.y, p2.x, p2.y);
}

// Bresenham's line algorithm
// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
void RaytracerBackend::drawLine(QImage& framebuffer, int x0, int y0, const int x1, const int y1, const uint color) {
    const auto dx = abs(x1 - x0);
    const auto dy = abs(y1 - y0);
    const auto sx = (x0 < x1) ? 1 : -1;
    const auto sy = (y0 < y1) ? 1 : -1;
    auto err = dx - dy;

    while (true) {
       if (x0 >= 0 &&
           y0 >= 0 &&
           x0 < framebuffer.width() &&
           y0 < framebuffer.height()) {
            framebuffer.setPixel(x0, y0, color);
       }

       if ((x0 == x1) && (y0 == y1)) break;
       const auto e2 = 2 * err;
       if (e2 > -dy) { err -= dy; x0 += sx; }
       if (e2 < dx) { err += dx; y0 += sy; }
    }
}

void RaytracerBackend::switchChanged() {
    if (m_lighting == LightingModel::Phong) {
        m_lighting = LightingModel::BlinnPhong;

    } else if (m_lighting == LightingModel::BlinnPhong) {
        m_lighting = LightingModel::Phong;
    }
}
