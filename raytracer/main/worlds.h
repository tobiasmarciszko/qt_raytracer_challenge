#ifndef WORLDS_H
#define WORLDS_H

#include "world.h"
#include "plane.h"
#include "materials.h"

namespace Worlds  {

    inline World threeBallsOnAPlane() {
        Plane floor;
        Material& m = floor.material;
        m.color = Color(0, 0.3, 0);
        m.reflective = 0.3;

        Plane sky;
        Material& m1 = sky.material;
        m1.ambient = 0.6;
        m1.diffuse = 0.6;
        m1.pattern_ptr = cloud_pattern();
        m1.pattern_ptr->set_transform(scaling(30, 30, 30));
        sky.set_transform(translation(0, 1000, 0));

        Plane wall;
        Material& mwall = wall.material;
        wall.set_transform(translation(0, 0, 4) * rotation_x(M_PI_2));
        mwall.color = Color(0, 0, 0);
        //mwall.reflective = 0.8;
        mwall.pattern_ptr = doomfire_pattern();
        mwall.pattern_ptr->set_transform(translation(0, -10, 0) * scaling(0.02, 0.04, 0.02) * rotation_x(M_PI_2));

        Sphere middle;
        middle.set_transform(translation(0, 1.2, 0));
        middle.material = Materials::Glass();

        Sphere right;
        right.set_transform(translation(1.5, 1, -0.5) * scaling(0.5, 0.5, 0.5));
        right.set_material(Materials::Glass());

        Sphere left;
        left.set_transform(translation(-1.5, 0.5, -0.75) * scaling(0.33, 0.33, 0.33));
        left.set_material(Materials::Glass());

        World world;

        world.lights.emplace_back(PointLight(Point(-30, 50, -100), Color(1, 1, 1)));
        // world.lights.emplace_back(PointLight(Point(2, 20, -100), Color(0.5, 0.5, 0.5)));
        // world.lights.emplace_back(PointLight(Point(0, 50, 0), Color(0.2, 0.2, 0.2)));

        world.shapes = {
            std::make_shared<Sphere>(middle),
            std::make_shared<Sphere>(right),
            std::make_shared<Sphere>(left),
            std::make_shared<Plane>(floor),
            std::make_shared<Plane>(sky),
//            std::make_shared<Plane>(wall)
        };

        return world;
    }

    inline World materialPreviewWorld() {
        World world;
        const auto light = PointLight(Point(-10, 10, -10), Color(1, 1, 1));
        world.lights = {light};

        auto sphere1 = std::make_shared<Sphere>(Sphere());
        sphere1->set_transform(translation(0, 0.5, -0.5) * scaling(0.5, 0.5, 0.5));
        Material m;
        m.color = Color(1, 0, 0);
        sphere1->set_material(m);

        auto floor = std::make_shared<Plane>(Plane());
        Material m1;
        m1.pattern_ptr = stripe_pattern(white, black);
        m1.pattern_ptr->set_transform(scaling(0.2, 1, 1));
        floor->set_material(m1);

        auto wall = std::make_shared<Plane>(Plane());
        wall->set_transform(translation(0, 0, 1) * rotation_x(M_PI_2));
        Material m2;
        m2.pattern_ptr = stripe_pattern(white, black);
        m2.pattern_ptr->set_transform(scaling(0.2, 0.2, 0.2) * rotation_x(M_PI_2) * rotation_z(M_PI_2));
        wall->set_material(m2);

        world.shapes = {sphere1, floor, wall};
        return world;
    }
}

#endif // WORLDS_H
