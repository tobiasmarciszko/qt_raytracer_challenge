#ifndef WORLDS_H
#define WORLDS_H

#include "world.h"
#include "plane.h"
#include "materials.h"
#include "light.h"
#include "sphere.h"
#include "patterns.h"
#include "cube.h"

inline Sphere glass_sphere() {
    Sphere s;
    s.set_transform(identity_matrix);
    s.material = Materials::glass;
    return s;
}

namespace Worlds  {

    inline World default_world()
    {
        World world;

        // Default world:
        const auto light = PointLight(Point(-10, 10, -10), Color(1, 1, 1));
        world.lights = {light};

        auto sphere1 = std::make_shared<Sphere>(Sphere());
        Material material;
        material.color = Color(0.8, 1.0, 0.6);
        material.diffuse = 0.7;
        material.specular = 0.2;
        sphere1->material = material;

        auto sphere2 = std::make_shared<Sphere>(Sphere());
        auto transform = scaling(0.5, 0.5, 0.5);
        sphere2->set_transform(transform);

        world.shapes = {sphere1, sphere2};

        return world;
    }

    inline World cubes() {

        World world;

        Plane floor;
        Material& m = floor.material;
        m.color = Color(0, 0.3, 0);
        m.reflective = 0.5;

        Plane sky;
        Material& m1 = sky.material;
        m1.ambient = 1;
        m1.diffuse = 1;
        m1.pattern_ptr = cloud_pattern();
        m1.pattern_ptr->set_transform(scaling(30, 30, 30));
        sky.set_transform(translation(0, 1000, 0));

        Plane wall;
        Material& mwall = wall.material;
        wall.set_transform(translation(0, 0, 5) * rotation_x(M_PI / 1.2));
        mwall.color = Color(0, 0, 0);
        //mwall.reflective = 0.8;
        mwall.transparency = 0.7;
        mwall.pattern_ptr = doomfire_pattern();
        mwall.pattern_ptr->set_transform(translation(0, 0, 0) * scaling(0.02, 0.04, 0.02) * rotation_x(M_PI_2));


        for (int i=-10; i<10; ++i) {
            Cube cube;
            cube.set_transform(translation(i, 1, 0) * scaling(0.5 * std::abs(i) * 0.1, 0.5, 0.5) * rotation_x(M_PI * (0.1 * i) / 3));
            cube.material.color = Color(std::abs(i) * 0.1, 0.2, 0.2);
            cube.material.reflective = 0.1 * std::abs(i);
            world.shapes.emplace_back(std::make_shared<Cube>(cube));
        }

        for (int i=-10; i<10; ++i) {
            Cube cube;
            cube.set_transform(translation(i, 3, 0) * scaling(0.5 * std::abs(i) * 0.1, 0.5, 0.5) * rotation_x(M_PI * (0.1 * i) / 3));
            cube.set_material(Materials::glass);
            world.shapes.emplace_back(std::make_shared<Cube>(cube));
        }

        world.shapes.emplace_back(std::make_shared<Plane>(floor));
        world.shapes.emplace_back(std::make_shared<Plane>(sky));

        world.lights.emplace_back(PointLight(Point(30, 50, -100), Color(0.9, 0.9, 0.9)));

        return world;
    }

    inline World threeBallsOnAPlane() {
        Plane floor;
        Material& m = floor.material;
        m.color = Color(0, 0.3, 0);
        m.reflective = 0.5;

        Plane sky;
        Material& m1 = sky.material;
        m1.ambient = 1;
        m1.diffuse = 1;
        m1.pattern_ptr = cloud_pattern();
        m1.pattern_ptr->set_transform(scaling(30, 30, 30));
        sky.set_transform(translation(0, 1000, 0));

        Plane wall;
        Material& mwall = wall.material;
        wall.set_transform(translation(0, 0, 5) * rotation_x(M_PI / 1.2));
        mwall.color = Color(0, 0, 0);
        //mwall.reflective = 0.8;
        mwall.transparency = 0.7;
        mwall.pattern_ptr = doomfire_pattern();
        mwall.pattern_ptr->set_transform(translation(0, 0, 0) * scaling(0.02, 0.04, 0.02) * rotation_x(M_PI_2));

        Sphere middle;
        middle.set_transform(translation(0, 1.2, 0));
        middle.material = Materials::diamond;

        Cube right;
        right.set_transform(translation(1.5, 1, -0.5) * scaling(0.5, 0.5, 0.5) * rotation_x(M_PI / 3));
        //right.set_material(Materials::glass);
        right.material.color = Color(0.7, 0.2, 0.2);
        right.material.reflective = 0.4;
        right.material.pattern_ptr = doomfire_pattern();
        right.material.pattern_ptr->set_transform(translation(0, 0, 0) * scaling(0.02, 0.04, 0.02) * rotation_x(M_PI_2));

        Cube left;
        left.set_transform(translation(-1.5, 0.5, -0.75) * scaling(0.33, 0.33, 0.33));
        //left.set_material(Materials::glass);
        left.material.reflective = 0.5;

        World world;

        world.lights.emplace_back(PointLight(Point(30, 50, -100), Color(0.9, 0.9, 0.9)));
        //world.lights.emplace_back(PointLight(Point(15, 45, -200), Color(0.5, 0.5, 0.5)));
        // world.lights.emplace_back(PointLight(Point(0, 50, 0), Color(0.2, 0.2, 0.2)));

        world.shapes = {
            std::make_shared<Sphere>(middle),
            std::make_shared<Cube>(right),
            std::make_shared<Cube>(left),
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
