#include "raytracer.h"
#include "canvas.h"
#include "color.h"
#include "point.h"
#include "vector.h"

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

projectile p(Point(0,1,0), Vector(10,100,0).normalize());
environment e(Vector(0, -0.0098, 0), Vector(0.01, 0, 0));

raytracer::raytracer() : m_canvas(Canvas(320,240))
{
    // Initialize framebuffer
    framebuffer = QImage(320, 240, QImage::Format_RGB32);
}

void raytracer::update()
{
    if (p.position.y() > 0) {
        auto pos = p.position + p.velocity;
        auto vel = p.velocity + e.gravity + e.wind;

        p = projectile(pos, vel);
        int x = p.position.x();
        x = x % 319;
        if (x < 0) x = 0;

        int y = 240 - p.position.y();
        y = y % 239;
        if (y < 0) y = 0;
        m_canvas.write_pixel(x, y, Color(1.0, 1.0, 1.0));
    }

    // Transfer canvas colors to framebuffer, maybe there is no need for an intermediate canvas?
    for (unsigned int i = 0; i < 320; ++i) {
        for (unsigned int j = 0; j < 240; ++j) {
            Color c = m_canvas.pixel_at(i, j);
            double r = c.red() * 255;
            double g = c.green() * 255;
            double b = c.blue() * 255;

            framebuffer.setPixel(i, j, qRgb(r, g, b));
        }
    }
}
