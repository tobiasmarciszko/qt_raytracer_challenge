#include <QDebug>

#include "tuple.h"
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


projectile tick(environment env, projectile proj)
{
    auto pos = proj.position + proj.velocity;
    auto vel = proj.velocity + env.gravity + env.wind;

    projectile newProj(pos, vel);
    return newProj;
}

int main()
{
    projectile p(Point(0,1,0), Vector(2,20,0).normalize());
    environment e(Vector(0, -0.01, 0), Vector(-0.00, 0, 0));

    while(p.position.y() > 0) {
        p = tick(e, p);
        qDebug() << "Position: x:" << p.position.x() << " y:" << p.position.y();
        // qDebug() << "Velocity: x:" << p.velocity.x() << " y:" << p.velocity.y();
    }
}

