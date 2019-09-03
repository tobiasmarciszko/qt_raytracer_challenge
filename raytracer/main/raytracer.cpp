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

#include <QRgb>
#include <QDebug>
#include <QElapsedTimer>

static int counter = 0;

World threeBallsOnAPlane();

Raytracer::Raytracer(QObject *parent) : QObject(parent)
{
    connect(&m_futureWatcher, SIGNAL(finished()), this, SLOT(renderFinished()));
    connect(&m_futureWatcher, SIGNAL(progressValueChanged(int)), this, SLOT(progressValueChanged(int)));

    m_world = threeBallsOnAPlane();
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

    qDebug() << "Rendering (" << m_camera.hsize << "x" << m_camera.vsize << ")" << QThread::idealThreadCount() << "threads";

    std::function<void(Pixel&)> renderPixel = [&](Pixel &pixel) {
        const Ray ray = ray_for_pixel(m_camera, pixel.x, pixel.y);
        pixel.color = color_at(m_world, ray, m_lighting);
    };

    m_timer.start();
    m_futureWatcher.setFuture(QtConcurrent::map(m_canvas.pixels, renderPixel));
}

void Raytracer::wireframe() {
    m_camera.transform = view_transform(Point(m_fromX, m_fromY, m_fromZ), Point(m_toX, m_toY, m_toZ), Vector(0, 1, 0));
    m_camera.inverse_transform = m_camera.transform.inverse();
    m_framebuffer.fill(QColor(0, 0, 0));

    for (auto& shape: m_world.shapes) {
        auto m = shape->transform();
        auto c = shape->material().color;

        // TODO: Refactoring into a color conversion method
        QColor qc;
        const auto r = c.red < 1.0 ? c.red : 1.0;
        const auto g = c.green < 1.0 ? c.green : 1.0;
        const auto b = c.blue < 1.0 ? c.blue : 1.0;
        qc.setRgbF(r, g, b);
        uint color = qRgb(qc.red(), qc.green(), qc.blue());

        auto centerPoint = m * Point(0, 0, 0);

        auto x = m.get(0,0);
        auto y = m.get(1,1);
        auto z = m.get(2,2);

        auto x1 = drawWorldPoint(Point(centerPoint.x+x, centerPoint.y, centerPoint.z));
        auto x2 = drawWorldPoint(Point(centerPoint.x-x, centerPoint.y, centerPoint.z));

        auto y1 = drawWorldPoint(Point(centerPoint.x, centerPoint.y+y,centerPoint.z));
        auto y2 = drawWorldPoint(Point(centerPoint.x, centerPoint.y-y,centerPoint.z));

        auto z1 = drawWorldPoint(Point(centerPoint.x,centerPoint.y, centerPoint.z+z));
        auto z2 = drawWorldPoint(Point(centerPoint.x,centerPoint.y, centerPoint.z-z));

        drawLine(x1, x2, color);
        drawLine(y1, y2, color);
        drawLine(z1, z2, color);
    }

    emit imageReady(m_framebuffer);
}

void Raytracer::renderFinished() {
    qDebug() << "Frame rendered in" << m_timer.elapsed() << "ms";

    m_timer.start();
    for (unsigned int y = 0; y < m_camera.vsize;  y++) {
        for (unsigned int x = 0; x < m_camera.hsize;  x++) {

            Color c = m_canvas.pixel_at(x, y).color;
            QColor color;
            const auto r = c.red < 1.0 ? c.red : 1.0;
            const auto g = c.green < 1.0 ? c.green : 1.0;
            const auto b = c.blue < 1.0 ? c.blue : 1.0;

            color.setRgbF(r, g, b);
            setPixel(x, y, qRgb(color.red(),color.green(), color.blue()));
        }
    }

    qDebug() << "Framebuffer copied in" << m_timer.elapsed() << "ms";

    m_rendering = false;
    emit renderingChanged();
    emit imageReady(m_framebuffer);
    QString filename = "render" + QString::number(counter) + ".png";
    m_framebuffer.save(filename, "PNG", 100);

    if (counter < 100) {
        counter++;
        doFire();
        render();
    }
}

// Convert world coordinate to screen space
// Returns the screen coordinates as a Point (ignore z)
Point Raytracer::drawWorldPoint(const Point& point, uint color) {

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

    // Clip before draw to avoid plotting outside the screen
    if (pRasterx >=0 &&
        pRastery >=0 &&
        pRasterx < m_camera.hsize &&
        pRastery < m_camera.vsize) {
        setPixel(pRasterx, pRastery, color);
    }

    // Return the pixel coordinates on the canvas/screen
    return Point(pRasterx, pRastery, 0);
}

void Raytracer::setPixel(int x, int y, uint color) {
    QRgb *pixel = (QRgb *)m_framebuffer.scanLine(y); // select row
    pixel += x; // select column
    *pixel = color;
}

void Raytracer::drawLine(const Point& p1, const Point& p2, uint color) {
    drawLine(p1.x, p1.y, p2.x, p2.y, color);
}

// Bresenham's line algorithm
// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
void Raytracer::drawLine(int x0, int y0, int x1, int y1, uint color) {
    auto dx = abs(x1 - x0);
    auto dy = abs(y1 - y0);
    auto sx = (x0 < x1) ? 1 : -1;
    auto sy = (y0 < y1) ? 1 : -1;
    auto err = dx - dy;

    while (true) {
       if (x0 >= 0 &&
           y0 >= 0 &&
           x0 < m_camera.hsize &&
           y0 < m_camera.vsize) {
            m_framebuffer.setPixel(x0, y0, color);
       }

       if ((x0 == x1) && (y0 == y1)) break;
       auto e2 = 2 * err;
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

inline World threeBallsOnAPlane() {
    auto floor = Plane();
    auto m = floor.material();
    m.color = Color(1, 0.2, 0.2);
//    m.pattern = stripe_pattern(black, white);
//    m.pattern->set_transform(scaling(0.1,0.1,0.1));
    floor.set_material(m);

    auto middle = Sphere();
    middle.set_transform(translation(0, 1, 0));
    auto m2 = Material();
    m2.color = Color(0.1, 1, 0.5);
    m2.diffuse = 0.7;
    m2.specular = 0.3;
    m2.pattern = stripe_pattern(black, white);
    m2.pattern->set_transform(translation(0, -0.9, 0) * scaling(0.01,0.01,0.01));
    middle.set_material(m2);

    auto right = Sphere();
    right.set_transform(translation(1.5, 0.5, -0.5) * scaling(0.5, 0.5, 0.5));
    auto m3 = Material();
    m3.color = Color(0.5, 1, 0.1);
    m3.diffuse = 0.7;
    m3.specular = 0.3;
    m3.pattern = stripe_pattern(black, white);
    m3.pattern->set_transform(translation(0, -0.9, 0) * scaling(0.01,0.01,0.01));
    right.set_material(m3);

    auto left = Sphere();
    left.set_transform(translation(-1.5, 0.33, -0.75) * scaling(0.33, 0.33, 0.33));
    auto m4 = Material();
    m4.color = Color(1, 0.8, 0.1);
    m4.diffuse = 0.7;
    m4.specular = 0.3;
    m4.pattern = stripe_pattern(black, white);
    m4.pattern->set_transform(translation(0, -0.9, 0) * scaling(0.01,0.01,0.01));
    left.set_material(m4);

    auto world = World();

    world.lights.emplace_back(PointLight(Point(-10, 10, -10), Color(1, 1, 1)));
//    world.lights.emplace_back(PointLight(Point(2, 2,-20), Color(0.4, 0.4, 0.4)));
//    world.lights.emplace_back(PointLight(Point(0, 200, 0), Color(0.2, 0.2, 0.2)));

    world.shapes = {
                    std::make_shared<Sphere>(middle),
                    std::make_shared<Sphere>(right),
                    std::make_shared<Sphere>(left),
                    std::make_shared<Plane>(floor)
    };

    return world;
}
