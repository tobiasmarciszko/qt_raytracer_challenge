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

raytracer::raytracer() : m_canvas(Canvas<320,240>())
{
    // Initialize framebuffer
    framebuffer = QImage(320, 240, QImage::Format_RGB32);
}

void raytracer::update()
{
    // QElapsedTimer timer;
    // timer.start();
    // qDebug() << "Start of effect";

// Pick one effect plz ;)
#if 0
    projectileEffect();
    clockEffect();
    flatSphere();
#endif
    shadedSphere();

    // qDebug() << "Time elapsed: " << timer.elapsed() << "ms";
}

void raytracer::projectileEffect() {
    if (p.position.y() > 0) {
        auto pos = p.position + p.velocity;
        auto vel = p.velocity + e.gravity + e.wind;

        p = projectile(pos, vel);
        int x = static_cast<int>(p.position.x());
        x = x % 319;
        if (x < 0) x = 0;

        int y = 240 - static_cast<int>(p.position.y());
        y = y % 239;
        if (y < 0) y = 0;
        writePixel(x, y, Color(1.0, 0.5, 1.0));
    }
}

void raytracer::clockEffect() {

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

        const auto x = point.x();
        const auto y = point.z();
        writePixel(static_cast<int>(x), static_cast<int>(y), Color(1.0, 1.0, 1.0));
    }
}

// First step in casting rays!
void raytracer::flatSphere()
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
            const auto xs = intersect(r, shape);
            const auto i = hit(xs);

            if (i.has_value()) {
                writePixel(x, y, Color(1, 0, 0));
            }
        }
    }
}

void raytracer::shadedSphere()
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

    // 1. Add material to the sphere
    auto material = shape.material();
    material.color = Color(0.5, 0.8, 0.4);
    shape.set_material(material);

    // 2. Add a light source!
    const auto light_position = Point(-10, 10, -10);
    const auto light_color = Color(1, 1, 1);
    const auto light = PointLight(light_position, light_color);

    // it can be transformed any way we want :)
    shape.set_transform(rotation_z(tick * M_PI/180) * shearing(0.2, 0, 0, 0, 0, 0));

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
            const auto xs = intersect(r, shape);
            const auto i = hit(xs);

            if (i.has_value()) {

                const auto point = r.position(i->t());
                const Sphere sphere = i->object();
                const auto normal = sphere.normal_at(point);
                const auto eye = -r.direction();
                const auto color = lighting(sphere.material(), light, point, eye, normal);

                writePixel(x, y, color);
            }
        }
    }
}


void raytracer::writePixel(const int x, const int y, const Color& c) {

    // Since we are using the canvas to populate our final image, and possible changing colors
    // We use it as a backing field, but the drawing itself on screen takes place in the framebuffer.

    m_canvas.write_pixel(static_cast<unsigned int>(x), static_cast<unsigned int>(y), c);
    QColor color;

    qreal r = c.red() < 1.0 ? c.red() : 1.0;
    qreal g = c.green() < 1.0 ? c.green() : 1.0;
    qreal b = c.blue() < 1.0 ? c.blue() : 1.0;

    color.setRgbF(r, g, b);
    framebuffer.setPixelColor(x, y, color);
}
