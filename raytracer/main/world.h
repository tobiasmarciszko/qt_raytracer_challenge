#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <memory>
#include "light.h"
#include "shape.h"

struct World
{
    // std::vector<AreaLight> lights{};
    std::vector<Light> lights{};
    std::vector<std::unique_ptr<Shape>> shapes{};

    [[nodiscard]] Shape* getShapePtrFromId(uint id) const {
        for(auto& shape_ptr: shapes) {
            if (shape_ptr->id == id) {
                return shape_ptr.get();
            }
        }

        return nullptr;
    }
};

#endif //WORLD_H
