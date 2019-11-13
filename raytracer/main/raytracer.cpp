#include "raytracer.h"
#include "canvas.h"
#include "color.h"
#include "point.h"
#include "vector.h"
#include "matrix.h"
#include "ray.h"
#include "sphere.h"
#include "light.h"
#include "lighting.h"
#include "intersection.h"
#include "camera.h"
#include "world.h"
#include "plane.h"
#include "worlds.h"

#include <QRgb>
#include <QDebug>
#include <QElapsedTimer>
#include <QPainter>

static int counter = 0;

Raytracer::Raytracer(QObject *parent) :
    QObject(parent),
    m_world(Worlds::threeBallsOnAPlane()),
    m_previewWorld(Worlds::materialPreviewWorld())
{
    connect(&m_futureWatcher, SIGNAL(finished()), this, SLOT(renderFinished()));
    connect(&m_materialPreviewfutureWatcher, SIGNAL(finished()), this, SLOT(materialPreviewFinished()));
    connect(&m_futureWatcher, SIGNAL(progressValueChanged(int)), this, SLOT(progressValueChanged(int)));
}

void Raytracer::progressValueChanged(int value)
{
    m_progress = value * 100 / m_canvas.pixels.size();
    emit progressChanged();
}

void Raytracer::setViewportSize(int width, int height) {

    if (width < 0 || height < 0) return;

    m_width = width;
    m_height = height;

    m_camera = Camera(m_width, m_height, M_PI / 3.0);
    m_camera.transform = view_transform(Point(m_fromX, m_fromY, m_fromZ), Point(m_toX, m_toY, m_toZ), Vector(0, 1, 0));
    m_camera.inverse_transform = m_camera.transform.inverse();
    m_framebuffer = QImage(m_width, m_height, QImage::Format_RGB32);
    m_canvas = Canvas(m_width, m_height);
}

void Raytracer::render() {

    m_futureWatcher.cancel();
    m_rendering = true;
    emit renderingChanged();

    qDebug() << "Rendering" << m_camera.hsize << "x" << m_camera.vsize << "using" << QThread::idealThreadCount() << "threads";

    std::function<void(Pixel&)> renderPixel = [&](Pixel &pixel) {
        const Ray ray = ray_for_pixel(m_camera, pixel.x, pixel.y);
        pixel.color = color_at(m_world, ray, m_lighting, 5);
    };

    m_timer.start();
    m_futureWatcher.setFuture(QtConcurrent::map(m_canvas.pixels, renderPixel));
}

void Raytracer::materialPreview() {

    m_materialPreviewfutureWatcher.cancel();

    m_previewCamera.transform = view_transform(Point(0, 1.0, -1.5), Point(0, 0, 0), Vector(0, 1, 0));
    m_previewCamera.inverse_transform = m_previewCamera.transform.inverse();

    m_previewWorld.shapes.at(0)->set_material(m_selectedMaterial);

    std::function<void(Pixel&)> renderPixel = [&](Pixel &pixel) {
        const Ray ray = ray_for_pixel(m_previewCamera, pixel.x, pixel.y);
        pixel.color = color_at(m_previewWorld, ray, m_lighting);
    };

    m_materialPreviewfutureWatcher.setFuture(QtConcurrent::map(m_previewCanvas.pixels, renderPixel));
}


void Raytracer::wireframe() {
    m_camera.transform = view_transform(Point(m_fromX, m_fromY, m_fromZ), Point(m_toX, m_toY, m_toZ), Vector(0, 1, 0));
    m_camera.inverse_transform = m_camera.transform.inverse();
    m_framebuffer.fill(QColor(100, 100, 100));

    for (auto& shape: m_world.shapes) {
        auto m = shape->transform();
        auto c = shape->material.color;

        // Only draw the bounding boxes on Spheres
        if (!dynamic_cast<Sphere*>(shape.get())) {
            continue;
        }

        // TODO: Refactoring into a color conversion method
        QColor qc;
        const auto r = c.red < 1.0 ? c.red : 1.0;
        const auto g = c.green < 1.0 ? c.green : 1.0;
        const auto b = c.blue < 1.0 ? c.blue : 1.0;
        qc.setRgbF(r, g, b);
        uint color = qRgb(qc.red(), qc.green(), qc.blue());

        const auto centerPoint = m * Point(0, 0, 0);

        const auto xScale = m.get(0,0);
        const auto yScale = m.get(1,1);
        const auto zScale = m.get(2,2);

        const auto xPos = centerPoint.x+xScale;
        const auto xNeg = centerPoint.x-xScale;

        const auto yPos = centerPoint.y+yScale;
        const auto yNeg = centerPoint.y-yScale;

        const auto zPos = centerPoint.z+zScale;
        const auto zNeg = centerPoint.z-zScale;

        // Top points
        const auto a1 = convertWorldToScreenPoint(Point(xPos, yPos, zNeg));
        const auto a2 = convertWorldToScreenPoint(Point(xNeg, yPos, zNeg));
        const auto a3 = convertWorldToScreenPoint(Point(xPos, yPos, zPos));
        const auto a4 = convertWorldToScreenPoint(Point(xNeg, yPos, zPos));

        // Bottom points
        const auto b1 = convertWorldToScreenPoint(Point(xPos, yNeg, zNeg));
        const auto b2 = convertWorldToScreenPoint(Point(xNeg, yNeg, zNeg));
        const auto b3 = convertWorldToScreenPoint(Point(xPos, yNeg, zPos));
        const auto b4 = convertWorldToScreenPoint(Point(xNeg, yNeg, zPos));

        // Top square
        drawLine(a1, a2, color);
        drawLine(a3, a4, color);
        drawLine(a1, a3, color);
        drawLine(a2, a4, color);

        // Bottom square
        drawLine(b1, b2, color);
        drawLine(b3, b4, color);
        drawLine(b1, b3, color);
        drawLine(b2, b4, color);

        // Connect top and bottom
        drawLine(a1, b1, color);
        drawLine(a2, b2, color);
        drawLine(a3, b3, color);
        drawLine(a4, b4, color);
    }

    emit imageReady(m_framebuffer);
}

void Raytracer::renderFinished() {
    m_lastRenderTime = static_cast<int>(m_timer.elapsed());
    m_timer.start();

    copyFrameBuffer();

    qDebug() << "Framebuffer copied in" << m_timer.elapsed() << "ms";

    m_rendering = false;
    emit renderingChanged();
    emit imageReady(m_framebuffer);
    QString filename = "render" + QString::number(counter) + ".png";
    m_framebuffer.save(filename, "PNG", 100);

    qDebug() << "Done";
    qDebug() << "";
}

void Raytracer::materialPreviewFinished() {
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

void Raytracer::copyFrameBuffer() {
    // Pointer to first pixel
    QRgb *data = reinterpret_cast<QRgb *>(m_framebuffer.bits());
    QColor color;

    for (const Pixel& pixel: m_canvas.pixels) {
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
Point Raytracer::convertWorldToScreenPoint(const Point& point, uint color) {

    // Transform point in world coordinate to camera
    auto pCamera = m_camera.transform * point;

    // Ignore points behind the camera
    if (pCamera.z > 0) return {-1, -1, -1};

    // Perspective divide
    auto pScreenx = pCamera.x / (pCamera.z);
    auto pScreeny = pCamera.y / -(pCamera.z);

    // Scale and offset acordingly according to the screen size
    auto p1xoffset = m_camera.half_width + pScreenx;
    auto p1yoffset = m_camera.half_height - pScreeny;
    auto pRasterx = ((p1xoffset / m_camera.pixel_size) - 0.5);
    auto pRastery = ((p1yoffset / m_camera.pixel_size) - 0.5);

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

void Raytracer::setPixel(int x, int y, uint color) {

// Raw access to pixel, I don't know if this is equivalent to using QPainter's "drawPoint"
#if 0
    QRgb *pixel = reinterpret_cast<QRgb *>(m_framebuffer.scanLine(y)); // select row
    pixel += x; // select column
    *pixel = color;
#endif
    QPainter p(&m_framebuffer);
    p.setPen(color);
    p.drawPoint(x, y);
}

void Raytracer::drawLine(const Point& p1, const Point& p2, uint color) {
    // drawLine(p1.x, p1.y, p2.x, p2.y, color);
    QPainter p(&m_framebuffer);
    p.setRenderHints(QPainter::Antialiasing);
    p.setPen(color);
    p.drawLine(p1.x, p1.y, p2.x, p2.y);
}

// Bresenham's line algorithm
// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
void Raytracer::drawLine(int x0, int y0, const int x1, const int y1, const uint color) {
    const auto dx = abs(x1 - x0);
    const auto dy = abs(y1 - y0);
    const auto sx = (x0 < x1) ? 1 : -1;
    const auto sy = (y0 < y1) ? 1 : -1;
    auto err = dx - dy;

    while (true) {
       if (x0 >= 0 &&
           y0 >= 0 &&
           x0 < m_camera.hsize &&
           y0 < m_camera.vsize) {
            m_framebuffer.setPixel(x0, y0, color);
       }

       if ((x0 == x1) && (y0 == y1)) break;
       const auto e2 = 2 * err;
       if (e2 > -dy) { err -= dy; x0 += sx; }
       if (e2 < dx) { err += dx; y0 += sy; }
    }
}

void Raytracer::switchChanged() {
    if (m_lighting == LightingModel::Phong) {
        m_lighting = LightingModel::BlinnPhong;

    } else if (m_lighting == LightingModel::BlinnPhong) {
        m_lighting = LightingModel::Phong;
    }
}
