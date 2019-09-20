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

    qDebug() << "Rendering" << m_camera.hsize << "x" << m_camera.vsize << "using" << QThread::idealThreadCount() << "threads";

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
    m_framebuffer.fill(QColor(100, 100, 100));

    for (auto& shape: m_world.shapes) {
        auto m = shape->transform();
        auto c = shape->material().color;

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
        const auto a1 = drawWorldPoint(Point(xPos, yPos, zNeg));
        const auto a2 = drawWorldPoint(Point(xNeg, yPos, zNeg));
        const auto a3 = drawWorldPoint(Point(xPos, yPos, zPos));
        const auto a4 = drawWorldPoint(Point(xNeg, yPos, zPos));

        // Bottom points
        const auto b1 = drawWorldPoint(Point(xPos, yNeg, zNeg));
        const auto b2 = drawWorldPoint(Point(xNeg, yNeg, zNeg));
        const auto b3 = drawWorldPoint(Point(xPos, yNeg, zPos));
        const auto b4 = drawWorldPoint(Point(xNeg, yNeg, zPos));

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

    qDebug() << "Framebuffer copied in" << m_timer.elapsed() << "ms";

    m_rendering = false;
    emit renderingChanged();
    emit imageReady(m_framebuffer);
    QString filename = "render" + QString::number(counter) + ".png";
    m_framebuffer.save(filename, "PNG", 100);

//    if (counter < 100) {
//        counter++;
//        doFire();
//        render();
//    }
    qDebug() << "Done";
    qDebug() << "";
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
    QRgb *pixel = reinterpret_cast<QRgb *>(m_framebuffer.scanLine(y)); // select row
    pixel += x; // select column
    *pixel = color;
}

void Raytracer::drawLine(const Point& p1, const Point& p2, uint color) {
    drawLine(p1.x, p1.y, p2.x, p2.y, color);
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

inline World threeBallsOnAPlane() {
    Plane floor;
    Material m = floor.material();
    m.reflective = 0.5;
    m.color = Color(1, 0.2, 0.2);
    //m.pattern_ptr = stripe_pattern(black, white);
    floor.set_material(m);

    Plane sky;
    Material m1 = sky.material();
    sky.set_transform(translation(0, 200, 0) * rotation_z(M_PI));
    m1.color = Color(0.2, 0.2, 0.9);
    sky.set_material(m1);

    Plane wall;
    Material mwall = wall.material();
    wall.set_transform(translation(0, 0, 4) * rotation_x(M_PI_2));
    mwall.color = Color(0, 0, 0);
    mwall.reflective = 0.8;
    mwall.pattern_ptr = doomfire_pattern();
    mwall.pattern_ptr->set_transform(scaling(0.02, 0.02, 0.02) * rotation_x(M_PI_2));

    wall.set_material(mwall);

    Sphere middle;
    middle.set_transform(translation(0, 1.2, 0));
    Material m2;
    m2.color = Color(0, 0, 0);
    m2.diffuse = 0.7;
    m2.specular = 0.9;
    m2.reflective = 0.6;
//    m2.pattern_ptr = doomfire_pattern();
//    m2.pattern_ptr->set_transform(translation(0, -0.9, 0) * scaling(0.01,0.01,0.01));
    middle.set_material(m2);

    Sphere right;
    right.set_transform(translation(1.5, 1, -0.5) * scaling(0.5, 0.5, 0.5));
    Material m3;
    m3.color = Color(0.5, 1, 0.1);
    m3.diffuse = 0.7;
    m3.specular = 0.3;
    m3.reflective = 1.0;
//    m3.pattern_ptr = xor_pattern();
//    m3.pattern_ptr->set_transform(translation(0, -0.9, 0) * scaling(0.01,0.01,0.01));
    right.set_material(m3);

    Sphere left;
    left.set_transform(translation(-1.5, 0.5, -0.75) * scaling(0.33, 0.33, 0.33));
    Material m4;
    m4.color = Color(1, 0.8, 0.1);
    m4.diffuse = 0.3;
    m4.specular = 0.2;
    m4.reflective = 0.7;
//    m4.pattern_ptr = stripe_pattern(Color(0.1, 0.1, 0.8), white);
//    m4.pattern_ptr->set_transform(translation(0, -0.9, 0) * scaling(0.02,0.02,0.02));
    left.set_material(m4);

    World world;

    world.lights.emplace_back(PointLight(Point(-3, 3, -3), Color(1, 1, 1)));
    // world.lights.emplace_back(PointLight(Point(2, 2,-10), Color(1, 1, 1)));
    // world.lights.emplace_back(PointLight(Point(0, 50, 0), Color(0.2, 0.2, 0.2)));

    world.shapes = {
                    std::make_shared<Sphere>(middle),
                    std::make_shared<Sphere>(right),
                    std::make_shared<Sphere>(left),
                    std::make_shared<Plane>(floor),
                    std::make_shared<Plane>(sky),
                    std::make_shared<Plane>(wall)
    };

    return world;
}
