#include <QCoreApplication>
#include <QDebug>

#include "tuple.h"
#include "point.h"
#include "vector.h"

class projectile {
public:
    projectile(Point pos, Vector v) : position(pos), velocity(v) {}
    Point position;
    Vector velocity;
};

class environment {
public:
    environment(Vector g, Vector w): gravity(g), wind(w) {}
    Vector gravity;
    Vector wind;
};


projectile tick(environment env, projectile proj)
{
    Point pos = proj.position + proj.velocity;
    Vector vel = proj.velocity + env.gravity + env.wind;

    projectile newProj(pos, vel);
    return newProj;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    projectile p(Point(0,1,0), Vector(1,1,0).normalize());
    environment e(Vector(0, -0.1, 0), Vector(-0.01, 0, 0));

    for(int i = 0; i < 100; i++) {
        p = tick(e, p);
        qDebug() << "Position: x:" << p.position.x() << " y:" << p.position.y();
        qDebug() << "Velocity: x:" << p.velocity.x() << " y:" << p.velocity.y();

        if (p.position.y() < 0) break;
    }

    return a.exec();
}

