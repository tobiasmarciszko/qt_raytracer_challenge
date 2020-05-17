#ifndef WORLDS_H
#define WORLDS_H

#include "world.h"
#include "plane.h"
#include "materials.h"
#include "light.h"
#include "sphere.h"
#include "patterns.h"
#include "cube.h"
#include "cylinder.h"

#include <QRandomGenerator>

inline Sphere glass_sphere() {
    Sphere s;
    s.set_transform(identity_matrix);
    s.material = Materials::glass;
    return s;
}

namespace Worlds  {

inline World cornell_box() {

    Plane left_wall;
    Plane right_wall;
    Plane back_wall;
    Plane floor;
    Plane ceiling;

    Cube cube;
    Sphere sphere;
    sphere.set_material(Materials::glass);
    Sphere sphere2;
    sphere2.material.reflective = 1.0;

    Material wall_material;
    wall_material.ambient = 0.2;
    wall_material.specular = 0.0;
    wall_material.diffuse = 0.5;

    left_wall.set_material(wall_material);
    right_wall.set_material(wall_material);
    back_wall.set_material(wall_material);
    floor.set_material(wall_material);
    ceiling.set_material(wall_material);
    cube.set_material(wall_material);

    left_wall.material.color = Color(1.0, 0.0, 0.0);
    right_wall.material.color = Color(0.0, 1.0, 0.0);
    back_wall.material.color = Color(1.0, 1.0, 1.0);
    floor.material.color = Color(1.0, 1.0, 1.0);
    ceiling.material.color = Color(1.0, 1.0, 1.0);
    
    left_wall.set_transform(translation(-1, 0, 0) * rotation_z(M_PI_2));
    right_wall.set_transform(translation(1, 0, 0) * rotation_z(-M_PI_2));
    back_wall.set_transform(translation(0, 0, 1) * rotation_x(M_PI_2));
    floor.set_transform(translation(0, -1, 0));
    ceiling.set_transform(translation(0, 1, 0) * rotation_z(M_PI));
    cube.set_transform(translation(-0.3, -0.5, -1.0) * scaling(0.25, 0.5, 0.25) * rotation_y(M_PI / 3));
    sphere.set_transform(translation(0.6, -0.75, -2.5) * scaling(0.25, 0.25, 0.25));
    sphere2.set_transform(translation(-0.6, -0.75, -2.5) * scaling(0.25, 0.25, 0.25));


    // PointLight light = PointLight(Point(0, 0.8, -2), Color(1.0, 1.0, 1.0));
    AreaLight light = AreaLight{Point{0, 0.9, -2}, Vector{0.25, 0, 0}, 8, Vector{0, 0, 0.25}, 8, Color{1, 1, 1}};

    World world;
    world.lights = {light};

    world.shapes.emplace_back(std::make_unique<Plane>(left_wall));
    world.shapes.emplace_back(std::make_unique<Plane>(right_wall));
    world.shapes.emplace_back(std::make_unique<Plane>(back_wall));
    world.shapes.emplace_back(std::make_unique<Plane>(floor));
    world.shapes.emplace_back(std::make_unique<Plane>(ceiling));
    world.shapes.emplace_back(std::make_unique<Cube>(cube));
    world.shapes.emplace_back(std::make_unique<Sphere>(sphere));
    world.shapes.emplace_back(std::make_unique<Sphere>(sphere2));

    return world;
}

inline World default_world()
{
    World world;

    // Default world:
    const auto light = PointLight(Point(-10, 10, -10), Color(1, 1, 1));
    world.lights = {light};

    Sphere sphere1;
    Material material;
    material.color = Color(0.8, 1.0, 0.6);
    material.diffuse = 0.7;
    material.specular = 0.2;
    sphere1.material = material;

    Sphere sphere2;
    auto transform = scaling(0.5, 0.5, 0.5);
    sphere2.set_transform(transform);

    world.shapes.emplace_back(std::make_unique<Sphere>(sphere1));
    world.shapes.emplace_back(std::make_unique<Sphere>(sphere2));

    return world;
}

//inline World cubes() {
//
//    World world;
//
//    Plane floor;
//    Material& m = floor.material;
//    m.color = Color(0, 0.3, 0);
//    m.reflective = 0.5;
//
//    Plane sky;
//    Material& m1 = sky.material;
//    m1.ambient = 1;
//    m1.diffuse = 1;
//    m1.pattern_ptr = cloud_pattern();
//    m1.pattern_ptr->set_transform(scaling(30, 30, 30));
//    sky.set_transform(translation(0, 1000, 0));
//
//    Plane wall;
//    Material& mwall = wall.material;
//    wall.set_transform(translation(0, 0, 5) * rotation_x(M_PI / 1.2));
//    mwall.color = Color(0, 0, 0);
//    //mwall.reflective = 0.8;
//    mwall.transparency = 0.7;
//    mwall.pattern_ptr = doomfire_pattern();
//    mwall.pattern_ptr->set_transform(translation(0, 0, 0) * scaling(0.02, 0.04, 0.02) * rotation_x(M_PI_2));
//
//
//    for (int i=-10; i<10; ++i) {
//        Cube cube;
//        cube.set_transform(translation(i, 1, 0) * scaling(0.5 * std::abs(i) * 0.1, 0.5, 0.5) * rotation_x(M_PI * (0.1 * i) / 3));
//        cube.material.color = Color(std::abs(i) * 0.1, 0.2, 0.2);
//        cube.material.reflective = 0.1 * std::abs(i);
//        world.shapes.emplace_back(std::make_shared<Cube>(cube));
//    }
//
//    for (int i=-10; i<10; ++i) {
//        Cube cube;
//        cube.set_transform(translation(i, 3, 0) * scaling(0.5 * std::abs(i) * 0.1, 0.5, 0.5) * rotation_x(M_PI * (0.1 * i) / 3));
//        cube.set_material(Materials::glass);
//        world.shapes.emplace_back(std::make_shared<Cube>(cube));
//    }
//
//    world.shapes.emplace_back(std::make_shared<Plane>(floor));
//    world.shapes.emplace_back(std::make_shared<Plane>(sky));
//
//    world.lights.emplace_back(PointLight(Point(30, 50, -100), Color(0.9, 0.9, 0.9)));
//
//    return world;
//}
//
//inline World threeBallsOnAPlane() {
//    Plane floor;
//    Material& m = floor.material;
//    m.color = Color(0, 0.3, 0);
//    m.reflective = 0.5;
//
//    Plane sky;
//    Material& m1 = sky.material;
//    m1.ambient = 1;
//    m1.diffuse = 1;
//    m1.pattern_ptr = cloud_pattern();
//    m1.pattern_ptr->set_transform(scaling(30, 30, 30));
//    sky.set_transform(translation(0, 1000, 0));
//
//    Cube wall;
//    Material& mwall = wall.material;
//    wall.set_transform(translation(0, 1.5, 5) * scaling(10, 2, 0.01));
//    mwall.color = Color(0, 0, 0);
//    mwall.pattern_ptr = doomfire_pattern();
//    mwall.pattern_ptr->set_transform(translation(0, -1, 0) * scaling(0.002, 0.015, 0.02));
//
//    Sphere middle;
////        middle.minimum = 0;
////        middle.maximum = 3;
////        middle.closed = true;
//    middle.set_transform(translation(0, 1, 0));
//    middle.material = Materials::glass;
//
//    Cylinder right;
//    right.minimum = -2;
//    right.maximum = 5;
//    right.closed = true;
//    right.set_transform(translation(1.5, 1, -0.5) * scaling(0.5, 0.5, 0.5) * rotation_x(M_PI / 3));
//    //right.set_material(Materials::glass);
////        right.material.color = Color(0.7, 0.2, 0.2);
////        right.material.reflective = 0.4;
////        right.material.pattern_ptr = doomfire_pattern();
////        right.material.pattern_ptr->set_transform(translation(0, 0, 0) * scaling(0.02, 0.04, 0.02) * rotation_x(M_PI_2));
//
//    Cube left;
//    // left.minimum = -1;
//    // left.maximum = 1;
//    // left.closed = true;
//    left.set_transform(translation(-1.5, 0.5, -0.75) * scaling(0.33, 0.33, 0.33));
//    //left.set_material(Materials::glass);
//    left.material.reflective = 0.5;
//
//    World world;
//
//    world.lights.emplace_back(PointLight(Point(30, 50, -100), Color(0.9, 0.9, 0.9)));
//    //world.lights.emplace_back(PointLight(Point(15, 45, -200), Color(0.5, 0.5, 0.5)));
//    // world.lights.emplace_back(PointLight(Point(0, 50, 0), Color(0.2, 0.2, 0.2)));
//
//    world.shapes = {
//        std::make_shared<Sphere>(middle),
//        std::make_shared<Cylinder>(right),
//        std::make_shared<Cube>(left),
//        std::make_shared<Plane>(floor),
//        std::make_shared<Plane>(sky)
//        // std::make_shared<Cube>(wall)
//    };
//
//    return world;
//}

inline World materialPreviewWorld() {
    World world;
    const auto light = PointLight(Point(-10, 10, -10), Color(1, 1, 1));
    // world.lights = {light};

    // std::unique_ptr<Sphere> sphere1 = std::make_unique<Sphere>(Sphere());
    Sphere sphere1;
    sphere1.set_transform(translation(0, 0.5, -0.5) * scaling(0.5, 0.5, 0.5));
    Material m;
    m.color = Color(1, 0, 0);
    sphere1.set_material(m);

    // std::unique_ptr<Plane> floor = std::make_unique<Plane>(Plane());
    Plane floor;
    Material m1;
    m1.pattern_ptr = stripe_pattern(white, black);
    m1.pattern_ptr->set_transform(scaling(0.2, 1, 1));
    floor.set_material(m1);

    // std::unique_ptr<Plane> wall = std::make_unique<Plane>(Plane());
    Plane wall;
    wall.set_transform(translation(0, 0, 1) * rotation_x(M_PI_2));
    Material m2;
    m2.pattern_ptr = stripe_pattern(white, black);
    m2.pattern_ptr->set_transform(scaling(0.2, 0.2, 0.2) * rotation_x(M_PI_2) * rotation_z(M_PI_2));
    wall.set_material(m2);

    world.shapes.emplace_back(std::make_unique<Sphere>(sphere1));
    world.shapes.emplace_back(std::make_unique<Plane>(floor));
    world.shapes.emplace_back(std::make_unique<Plane>(wall));

    return world;
}
/*
inline World randomCubes() {

    World world;

    Plane floor;
    Material& m = floor.material;
    floor.set_transform(translation(0, -10, 0));
    m.color = Color(0.5, 0.5, 0.5);
    m.ambient = 1;
    m.diffuse = 1;
    m.specular = 0.9;
    m.reflective = 0.2;

    Plane sky;
    Material& m1 = sky.material;
    m1.ambient = 1;
    m1.diffuse = 1;
    m1.pattern_ptr = cloud_pattern();
    m1.pattern_ptr->set_transform(scaling(15, 15, 15));
    sky.set_transform(translation(0, 200, 0));

    QRandomGenerator rd;

    auto numCubes = 400; //rd.bounded(20, 60);

    for (int i=0; i<numCubes; ++i) {

        auto x = rd.bounded(0, 100) - 50;
        auto y = rd.bounded(0, 100) - 10;
        auto z = rd.bounded(0, 400);
        auto color_r = rd.bounded(1.0F);
        auto color_g = rd.bounded(1.0F);
        auto color_b = rd.bounded(1.0F);

        auto ambient = rd.bounded(0.8F);
        auto diffuse = rd.bounded(0.8F);
        auto specular = rd.bounded(1.0F);

        auto transparency = rd.bounded(1.0F);
        auto reflective = rd.bounded(0.5F);
        auto refractive_index = rd.bounded(5.0F);

        auto scale_x = rd.bounded(2.0F) + 0.1F;
        auto scale_y = rd.bounded(2.0F) + 0.1F;
        auto scale_z = rd.bounded(2.0F) + 0.1F;

        auto rot_x = rd.bounded(M_2_PI);
        auto rot_y = rd.bounded(M_2_PI);
        auto rot_z = rd.bounded(M_2_PI);

        Cube cube;
        cube.set_transform(translation(x, y, z) * rotation_x(rot_x) * rotation_y(rot_y) * rotation_z(rot_z) * scaling(scale_x, scale_y, scale_z));
        cube.material.color = Color(color_r, color_g, color_b);
        cube.material.ambient = ambient;
        cube.material.diffuse = diffuse;
        cube.material.specular = specular;
        cube.material.reflective = reflective;
//            cube.material.transparency = transparency;
//            cube.material.refractive_index = refractive_index;

        world.shapes.emplace_back(std::make_shared<Cube>(cube));
    }

    world.shapes.emplace_back(std::make_shared<Plane>(floor));
    world.shapes.emplace_back(std::make_shared<Plane>(sky));

    world.lights.emplace_back(PointLight(Point(30, 50, -100), Color(0.85, 0.85, 0.85)));

    return world;
}*/

} // end namespace worlds

#endif // WORLDS_H
