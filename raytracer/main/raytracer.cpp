#include "raytracer.h"
#include "canvas.h"
#include "color.h"
#include "point.h"
#include "vector.h"
#include "matrix.h"

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
    // projectileEffect();
    clockEffect();

    copyCanvasToFramebuffer();
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
        m_canvas.write_pixel(static_cast<unsigned int>(x), static_cast<unsigned int>(y), Color(1.0, 0.5, 1.0));
    }
}

void raytracer::clockEffect() {

    // X and Z is in the 2D plane, Y is depth

    tick++;
    m_canvas.fill(Color(0, 0, 0));

    for (int i = 0; i < 12; i++) {
        Point point = Point(0, 0, 1);
        const auto rotate = rotation_y(i * M_PI/6 + tick);

        point = rotate * point;
        point = point * 80.0;
        point = point + Point(160, 0, 120);

        const auto x = point.x();
        const auto y = point.z();
        m_canvas.write_pixel(static_cast<int>(x), static_cast<int>(y), Color(1.0, 1.0, 1.0));
    }
}

void raytracer::copyCanvasToFramebuffer() {
    for (unsigned int i = 0; i < 320; ++i) {
        for (unsigned int j = 0; j < 240; ++j) {
            const Color c = m_canvas.pixel_at(i, j);
            const auto r = static_cast<int>(c.red() * 255);
            const auto g = static_cast<int>(c.green() * 255);
            const auto b = static_cast<int>(c.blue() * 255);
            framebuffer.setPixel(static_cast<int>(i), static_cast<int>(j), qRgb(r, g, b));
        }
    }
}
