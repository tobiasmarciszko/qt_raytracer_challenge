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

#include <QRgb>
#include <QDebug>
#include <QElapsedTimer>


struct projectile {
    projectile(Point pos, Vector v) : position(pos), velocity(v) {}
    Point position;
    Vector velocity;
};

struct environment {
    environment(Vector g, Vector w): gravity(g), wind(w) {}
    Vector gravity;
    Vector wind;
};

static projectile p(Point(0,1,0), Vector(10,100,0).normalize());
static environment e(Vector(0, -0.0098, 0), Vector(0.01, 0, 0));
static int tick = 0;

Raytracer::Raytracer(QObject *parent) : QObject(parent)
{
    connect(&m_futureWatcher, SIGNAL(finished()), this, SLOT(renderFinished()));
    connect(&m_futureWatcher, SIGNAL(progressValueChanged(int)), this, SLOT(progressValueChanged(int)));

    threeBallsOnAFloor();
}

void Raytracer::progressValueChanged(int value)
{
    emit progressChanged();
}

void Raytracer::update()
{
// Pick one effect plz ;)
#if 0
    projectileEffect();
    clockEffect();
    flatSphere();
    shadedSphere();
    defaultWorld();
    threeBallsOnAFloor();
#endif

    m_futureWatcher.cancel();
    render();
}

void Raytracer::projectileEffect() {
    if (p.position.y > 0) {
        auto pos = p.position + p.velocity;
        auto vel = p.velocity + e.gravity + e.wind;

        p = projectile(pos, vel);
        int x = static_cast<int>(p.position.x);
        x = x % 319;
        if (x < 0) x = 0;

        int y = 240 - static_cast<int>(p.position.y);
        y = y % 239;
        if (y < 0) y = 0;
        // writePixel(x, y, Color(1.0, 0.5, 1.0));
    }
}

void Raytracer::clockEffect() {

    // X and Z is in the 2D plane, Y is depth

    tick++;
    m_canvas.fill(Color(0, 0, 0));
    framebuffer.fill(qRgb(0, 0, 0));

    for (int i = 0; i < 12; i++) {
        Point point = Point(0, 0, 1);
        const auto rotate = rotation_y(i * M_PI/6 + tick);

        point = rotate * point;
        point = point * 80.0;
        point = point + Point(160, 0, 120);

        const auto x = point.x;
        const auto y = point.z;
        // writePixel(static_cast<int>(x), static_cast<int>(y), Color(1.0, 1.0, 1.0));
    }
}

// First step in casting rays!
void Raytracer::flatSphere()
{
    tick++;
    m_canvas.fill(Color(0, 0, 0));
    framebuffer.fill(qRgb(0, 0, 0));

    const auto ray_origin = Point(0, 0, -5);
    const auto wall_z = 10;
    const auto wall_size = 7.0;

    const auto canvas_pixels = 240; // (assuming 240 by 240)
    const auto pixel_size = wall_size / canvas_pixels;

    const auto half = wall_size / 2;

    auto shape = Sphere();

    // it can be transformed any way we want :)
    shape.set_transform(rotation_z(tick * M_PI/180) * shearing(1, 0, 0, 0, 0, 0) * scaling(0.0 + (tick/1000.0), 0.0 + (tick/1000.0), 0.0 + (tick/1000.0)));

    // for every pixel row in the canvas
    for(int y = 0; y < canvas_pixels -1; y++) {

        // compute world y coordinate
        const auto world_y = half - pixel_size * y;

        // for each pixel in the row!
        for (int x = 0; x < canvas_pixels -1; x++) {
            const auto world_x = - half + pixel_size * x;

            // the target point on the wall that the ray aim for
            const auto position = Point(world_x, world_y, wall_z);

            // the ray is pointing from its origin to the position on the wall
            const Vector ray_direction = position - ray_origin;

            const auto r = Ray(ray_origin, ray_direction.normalize());

            // will the ray hit the sphere?
            const auto xs = shape.intersect(r);
            const auto i = hit(xs);

            if (i.has_value()) {
                // writePixel(x, y, Color(1, 0, 0));
            }
        }
    }
}

void Raytracer::shadedSphere()
{
    tick++;
    m_canvas.fill(Color(0, 0, 0));
    framebuffer.fill(qRgb(0, 0, 0));

    const auto ray_origin = Point(0, 0, -5);
    const auto wall_z = 10;

    const auto wall_size_width = 13.3;
    const auto wall_size_height = 10.0;

    const auto canvas_pixels_width = 320;
    const auto canvas_pixels_height = 240;

    const auto pixel_size_width = wall_size_width / canvas_pixels_width;
    const auto pixel_size_height = wall_size_height / canvas_pixels_height;

    const auto half_width = wall_size_width / 2;
    const auto half_height = wall_size_height / 2;

    Sphere shape1 = Sphere();
    Sphere shape2 = Sphere();

    // 1. Add material to the sphere
    auto material = shape1.material();
    material.color = Color(0.5, 0.8, 0.4);
    shape1.set_material(material);

    auto material2= shape2.material();
    material2.color = Color(1, 0, 0);
    material2.specular = 0.3;
    material2.diffuse = 0.9;
    material2.shininess = 20.0;
    shape2.set_material(material2);

    // 2. Add a light source!
    const auto light_position = Point(50 * std::sin(M_PI * tick / 180), 50 * std::cos(M_PI * tick / 180), -5);
    const auto light_color = Color(1, 1, 1);
    const auto light = PointLight(light_position, light_color);

    const auto light_position2 = Point(50 * std::cos(M_PI * tick / 180), 50 * std::sin(M_PI * tick / 180), -5);
    const auto light_color2 = Color(1, 1, 1);
    const auto light2 = PointLight(light_position2, light_color2);

    // it can be transformed any way we want :)
    shape1.set_transform(rotation_z(tick * M_PI/180) * translation(0.5, 0.5, 0) * scaling(0.5, 0.5, 0.5));
    shape2.set_transform(rotation_z(tick * M_PI/180) * translation(-0.5, -0.5, 0) * scaling(0.5, 0.5, 0.5));

    // for every pixel row in the canvas
    for(int y = 0; y < canvas_pixels_height -1; y++) {

        // compute world y coordinate
        const auto world_y = half_height - pixel_size_height * y;

        // for each pixel in the row!
        for (int x = 0; x < canvas_pixels_width -1; x++) {
            const auto world_x = - half_width + pixel_size_width * x;

            // the target point on the wall that the ray aim for
            const auto position = Point(world_x, world_y, wall_z);

            // the ray is pointing from its origin to the position on the wall
            const Vector ray_direction = position - ray_origin;

            const auto r = Ray(ray_origin, ray_direction.normalize());

            // will the ray hit the sphere?
            auto xs = shape1.intersect(r);
            auto xs2 = shape2.intersect(r);

            auto i = hit(xs);

            if (!i.has_value()) {
                i = hit(xs2);
            }

            if (i.has_value()) {

                const auto point = r.position(i->t());
                auto shape = i->object();
                const auto normal = shape.get()->normal_at(point);
                const auto eye = -r.direction();
                const auto color = lighting(shape.get()->material(), light, point, eye, normal);
                const auto color2 = lighting(shape.get()->material(), light2, point, eye, normal);

                // writePixel(x, y, color + color2);
            }
        }
    }
}

void Raytracer::defaultWorld()
{
    auto camera = Camera(320, 240, M_PI_4);
    auto world = default_world();

    const auto from = Point(0, 0, -5.0);
    const auto to = Point(0, 0, 0);
    const auto up = Vector(0, 1, 0);

    camera.transform = view_transform(from, to, up);

    // render(camera, world);
}

void Raytracer::threeBallsOnAFloor()
{
    auto floor = Sphere();
    floor.set_transform(scaling(10, 0.01, 10));
    auto m = floor.material();
    m.color = Color(1, 0.9, 0.9);
    m.specular = 0;
    floor.set_material(m);

    auto left_wall = Sphere();
    left_wall.set_transform(translation(0, 0, 5) * rotation_y(-M_PI_4) * rotation_x(M_PI_2) * scaling(10, 0.01, 10));
    left_wall.set_material(m);

    auto right_wall = Sphere();
    right_wall.set_transform(translation(0, 0, 5) * rotation_y(M_PI_4) * rotation_x(M_PI_2) * scaling(10, 0.01, 10));
    right_wall.set_material(m);

    auto middle = Sphere();
    middle.set_transform(translation(-0.5, 1, 0.5));
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
    world.lights.emplace_back(PointLight(Point(2, 2,-20), Color(0.4, 0.4, 0.4)));
    world.lights.emplace_back(PointLight(Point(0, 2000, 0), Color(0.2, 0.2, 0.2)));

    auto camera = Camera(320, 240, M_PI / 3);
    camera.transform = view_transform(Point(fromX, fromY, fromZ), Point(toX, toY, toZ), Vector(0, 1, 0));

    world.shapes = {std::make_shared<Sphere>(floor),
                    std::make_shared<Sphere>(left_wall),
                    std::make_shared<Sphere>(right_wall),
                    std::make_shared<Sphere>(middle),
                    std::make_shared<Sphere>(right),
                    std::make_shared<Sphere>(left)
                    };

    m_camera = camera;
    m_world = world;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

void Raytracer::render(int width, int height) {

    if (width < 0 || height < 0) return;

    m_camera = Camera(width, height, M_PI / 3);
    m_camera.transform = view_transform(Point(fromX, fromY, fromZ), Point(toX, toY, toZ), Vector(0, 1, 0));
    framebuffer = QImage(width, height, QImage::Format_RGB32);
    m_canvas = Canvas(width, height);

    render();
}

// Helper functions
void Raytracer::render() {

    auto camera = m_camera;
    auto world = m_world;

    setRendering(true);

    framebuffer.fill(qRgb(0, 0, 0));
    m_canvas.fill(Color(1,1,1));

    emit rendererReady(framebuffer);

    qDebug() << "Rendering (" << camera.hsize << "x" << camera.vsize << ")" << QThread::idealThreadCount() << "threads";

    m_timer.start();

    std::function<void(Color&)> renderPixel = [&](Color &color) {
        const Ray ray = ray_for_pixel(m_camera, color.x, color.y);
        color = color_at(m_world, ray, m_lighting);
    };

    m_futureWatcher.setFuture(QtConcurrent::map(m_canvas.m_pixels, renderPixel));
}

void Raytracer::renderFinished() {
    qDebug() << "Frame rendered in" << m_timer.elapsed() << "ms";

    m_timer.start();
    for (unsigned int y = 0; y < m_camera.vsize;  y++) {
        for (unsigned int x = 0; x < m_camera.hsize;  x++) {

            Color c = m_canvas.pixel_at(x, y);
            QColor color;
            const auto r = c.red < 1.0 ? c.red : 1.0;
            const auto g = c.green < 1.0 ? c.green : 1.0;
            const auto b = c.blue < 1.0 ? c.blue : 1.0;

            color.setRgbF(r, g, b);
            QRgb *pixel = (QRgb *)framebuffer.scanLine(y); // select row
            pixel += x; // select column
            *pixel = qRgb(color.red(),color.green(), color.blue());
        }
    }

    qDebug() << "Framebuffer copied in" << m_timer.elapsed() << "ms";

    emit rendererReady(framebuffer);
    setRendering(false);
}

void Raytracer::switchChanged() {
    if (m_lighting == LightingModel::Phong) {
        m_lighting = LightingModel::BlinnPhong;

    } else if (m_lighting == LightingModel::BlinnPhong) {
        m_lighting = LightingModel::Phong;
    }

    update();
}
