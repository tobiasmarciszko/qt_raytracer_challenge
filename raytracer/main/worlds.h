#ifndef WORLDS_H
#define WORLDS_H

#include "world.h"
#include "plane.h"

namespace Worlds  {

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

    inline World materialPreviewWorld() {
        World world;
        const auto light = PointLight(Point(-10, 10, -10), Color(1, 1, 1));
        world.lights = {light};

        auto sphere1 = std::make_shared<Sphere>(Sphere());
        Material m;
        m.color = Color(1, 0, 0);
        sphere1->set_material(m);

        world.shapes = {sphere1};
        return world;
    }
}

#endif // WORLDS_H
