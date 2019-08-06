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

    m_camera = Camera(m_width, m_height, M_PI / 3);
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
    // m_futureWatcher.setFuture(QtConcurrent::map(m_canvas.pixels, renderPixel));
    renderFinished();
}

void Raytracer::renderFinished() {
    qDebug() << "Frame rendered in" << m_timer.elapsed() << "ms";

    m_timer.start();
//    for (unsigned int y = 0; y < m_camera.vsize;  y++) {
//        for (unsigned int x = 0; x < m_camera.hsize;  x++) {

//            Color c = m_canvas.pixel_at(x, y).color;
//            QColor color;
//            const auto r = c.red < 1.0 ? c.red : 1.0;
//            const auto g = c.green < 1.0 ? c.green : 1.0;
//            const auto b = c.blue < 1.0 ? c.blue : 1.0;

//            color.setRgbF(r, g, b);
//            QRgb *pixel = (QRgb *)m_framebuffer.scanLine(y); // select row
//            pixel += x; // select column
//            *pixel = qRgb(color.red(),color.green(), color.blue());
//        }
//    }

    m_framebuffer.fill(QColor(0, 0, 0));

    auto p1 = m_camera.transform * Point(-0.1, 0.1, 0.1);
    auto p2 = m_camera.transform * Point(0.1, 0.1, 0.1);
    auto p3 = m_camera.transform * Point(-0.1, -0.1, 0.1);
    auto p4 = m_camera.transform * Point(-0.1, -0.1, -0.1);

    auto p5 = m_camera.transform * Point(-0.1, 0.1, -0.1);
    auto p6 = m_camera.transform * Point(0.1, 0.1, -0.1);
    auto p7 = m_camera.transform * Point(0.1, -0.1, 0.1);
    auto p8 = m_camera.transform * Point(0.1, -0.1, -0.1);

    auto p1xoffset = m_camera.half_width - p1.x;
    auto p1yoffset = m_camera.half_height - p1.y;
    auto p1x = ((p1xoffset / m_camera.pixel_size) - 0.5);
    auto p1y = ((p1yoffset / m_camera.pixel_size) - 0.5);

    auto p2xoffset = m_camera.half_width - p2.x;
    auto p2yoffset = m_camera.half_height - p2.y;
    auto p2x = ((p2xoffset / m_camera.pixel_size) - 0.5);
    auto p2y = ((p2yoffset / m_camera.pixel_size) - 0.5);

    auto p3xoffset = m_camera.half_width - p3.x;
    auto p3yoffset = m_camera.half_height - p3.y;
    auto p3x = ((p3xoffset / m_camera.pixel_size) - 0.5);
    auto p3y = ((p3yoffset / m_camera.pixel_size) - 0.5);

    auto p4xoffset = m_camera.half_width - p4.x;
    auto p4yoffset = m_camera.half_height - p4.y;
    auto p4x = ((p4xoffset / m_camera.pixel_size) - 0.5);
    auto p4y = ((p4yoffset / m_camera.pixel_size) - 0.5);

    auto p5xoffset = m_camera.half_width - p5.x;
    auto p5yoffset = m_camera.half_height - p5.y;
    auto p5x = ((p5xoffset / m_camera.pixel_size) - 0.5);
    auto p5y = ((p5yoffset / m_camera.pixel_size) - 0.5);

    auto p6xoffset = m_camera.half_width - p6.x;
    auto p6yoffset = m_camera.half_height - p6.y;
    auto p6x = ((p6xoffset / m_camera.pixel_size) - 0.5);
    auto p6y = ((p6yoffset / m_camera.pixel_size) - 0.5);

    auto p7xoffset = m_camera.half_width - p7.x;
    auto p7yoffset = m_camera.half_height - p7.y;
    auto p7x = ((p7xoffset / m_camera.pixel_size) - 0.5);
    auto p7y = ((p7yoffset / m_camera.pixel_size) - 0.5);

    auto p8xoffset = m_camera.half_width - p8.x;
    auto p8yoffset = m_camera.half_height - p8.y;
    auto p8x = ((p8xoffset / m_camera.pixel_size) - 0.5);
    auto p8y = ((p8yoffset / m_camera.pixel_size) - 0.5);

    qDebug() << p1x << p1y;
    // qDebug() << p2x << p2y;

    m_framebuffer.setPixel(p1x, p1y, qRgb(255, 255, 255));
    m_framebuffer.setPixel(p1x+1, p1y, qRgb(255, 255, 255));
    m_framebuffer.setPixel(p1x+1, p1y+1, qRgb(255, 255, 255));
    m_framebuffer.setPixel(p1x, p1y+1, qRgb(255, 255, 255));

    m_framebuffer.setPixel(p2x, p2y, qRgb(255, 255, 255));
    m_framebuffer.setPixel(p2x+1, p2y, qRgb(255, 255, 255));
    m_framebuffer.setPixel(p2x+1, p2y+1, qRgb(255, 255, 255));
    m_framebuffer.setPixel(p2x, p2y+1, qRgb(255, 255, 255));

    m_framebuffer.setPixel(p3x, p3y, qRgb(255, 255, 255));
    m_framebuffer.setPixel(p3x+1, p3y, qRgb(255, 255, 255));
    m_framebuffer.setPixel(p3x+1, p3y+1, qRgb(255, 255, 255));
    m_framebuffer.setPixel(p3x, p3y+1, qRgb(255, 255, 255));

    m_framebuffer.setPixel(p4x, p4y, qRgb(255, 255, 255));
    m_framebuffer.setPixel(p4x+1, p4y, qRgb(255, 255, 255));
    m_framebuffer.setPixel(p4x+1, p4y+1, qRgb(255, 255, 255));
    m_framebuffer.setPixel(p4x, p4y+1, qRgb(255, 255, 255));

    m_framebuffer.setPixel(p5x, p5y, qRgb(255, 255, 255));
    m_framebuffer.setPixel(p5x+1, p5y, qRgb(255, 255, 255));
    m_framebuffer.setPixel(p5x+1, p5y+1, qRgb(255, 255, 255));
    m_framebuffer.setPixel(p5x, p5y+1, qRgb(255, 255, 255));

    m_framebuffer.setPixel(p6x, p6y, qRgb(255, 255, 255));
    m_framebuffer.setPixel(p6x+1, p6y, qRgb(255, 255, 255));
    m_framebuffer.setPixel(p6x+1, p6y+1, qRgb(255, 255, 255));
    m_framebuffer.setPixel(p6x, p6y+1, qRgb(255, 255, 255));

    m_framebuffer.setPixel(p7x, p7y, qRgb(255, 255, 255));
    m_framebuffer.setPixel(p7x+1, p7y, qRgb(255, 255, 255));
    m_framebuffer.setPixel(p7x+1, p7y+1, qRgb(255, 255, 255));
    m_framebuffer.setPixel(p7x, p7y+1, qRgb(255, 255, 255));

    m_framebuffer.setPixel(p8x, p8y, qRgb(255, 255, 255));
    m_framebuffer.setPixel(p8x+1, p8y, qRgb(255, 255, 255));
    m_framebuffer.setPixel(p8x+1, p8y+1, qRgb(255, 255, 255));
    m_framebuffer.setPixel(p8x, p8y+1, qRgb(255, 255, 255));

    qDebug() << "Framebuffer copied in" << m_timer.elapsed() << "ms";

    m_rendering = false;
    emit renderingChanged();
    emit imageReady(m_framebuffer);
    // m_framebuffer.save("render", "PNG", 100);
}

void Raytracer::switchChanged() {
    if (m_lighting == LightingModel::Phong) {
        m_lighting = LightingModel::BlinnPhong;

    } else if (m_lighting == LightingModel::BlinnPhong) {
        m_lighting = LightingModel::Phong;
    }
}

inline World threeBallsOnAPlane()
{
    auto floor = Plane();
    auto m = floor.material();
    m.color = Color(1, 0.2, 0.2);
    floor.set_material(m);

    auto middle = Sphere();
    middle.set_transform(translation(0, 0, 0));
    auto m2 = Material();
    m2.color = Color(0.1, 1, 0.5);
    m2.diffuse = 0.7;
    m2.specular = 0.3;
    middle.set_material(m2);

    auto right = Sphere();
    right.set_transform(translation(1.5, 0.5, -0.5) * scaling(0.5, 0.5, 0.5));
    auto m3 = Material();
    m3.color = Color(0.5, 1, 0.1);
    m3.diffuse = 0.7;
    m3.specular = 0.3;
    right.set_material(m3);

    auto left = Sphere();
    left.set_transform(translation(-1.5, 0.33, -0.75) * scaling(0.33, 0.33, 0.33));
    auto m4 = Material();
    m4.color = Color(1, 0.8, 0.1);
    m4.diffuse = 0.7;
    m4.specular = 0.3;
    left.set_material(m4);

    auto world = World();

    world.lights.emplace_back(PointLight(Point(-20, 20, -20), Color(0.7, 0.7, 0.7)));
//    world.lights.emplace_back(PointLight(Point(2, 2,-20), Color(0.4, 0.4, 0.4)));
//    world.lights.emplace_back(PointLight(Point(0, 2000, 0), Color(0.2, 0.2, 0.2)));

    world.shapes = {
                    std::make_shared<Sphere>(middle),
                    std::make_shared<Sphere>(right),
                    std::make_shared<Sphere>(left),
                    std::make_shared<Plane>(floor)

    };

    return world;
}
