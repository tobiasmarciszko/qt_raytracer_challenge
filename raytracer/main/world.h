#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <memory>
#include "light.h"
#include "shape.h"

struct World
{
    std::vector<Light> lights;
    std::vector<std::shared_ptr<Shape>> shapes;
};

#endif //WORLD_H
