#include "raytracer.h"
#include "canvas.h"
#include "color.h"
#include "point.h"
#include "vector.h"
#include "matrix.h"
#include "ray.h"
#include "sphere.h"

#include <QRgb>

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
// Pick one effect plz ;)
#if 0
    projectileEffect();
    clockEffect();
#endif
    flatSphere();
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
    const auto ray_origin = Point(0, 0, -5);
    const auto wall_z = 10;
    const auto wall_size = 7.0;

    const auto canvas_pixels = 240; // (assuming 240 by 240)
    const auto pixel_size = wall_size / canvas_pixels;

    const auto half = wall_size / 2;

    auto shape = Sphere();

    // it can be transformed any way we want :)
    shape.set_transform(rotation_z(M_PI_4) * shearing(1, 0, 0, 0, 0, 0) * scaling(0.5, 0.5, 0.5));

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
                writePixel(x, y, Color(1, 0, 0));
            }
        }
    }
}

void raytracer::writePixel(const int x, const int y, const Color& c) {

    // Since we are using the canvas to populate our final image, and possible changing colors
    // We use it as a backing field, but the drawing itself on screen takes place in the framebuffer.

    m_canvas.write_pixel(static_cast<unsigned int>(x), static_cast<unsigned int>(y), c);
    QColor color;
    color.setRgbF(c.red(), c.green(), c.blue());
    framebuffer.setPixelColor(x, y, color);
}
