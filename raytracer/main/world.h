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

    std::shared_ptr<Shape> getShapePtrFromId(uint id) {
        for(auto& shape_ptr: shapes) {
            if (shape_ptr->id == id) {
                return shape_ptr;
            }
        }

        return nullptr;
    }
};

#endif //WORLD_H
