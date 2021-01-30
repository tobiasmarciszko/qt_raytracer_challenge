#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <memory>
#include "light.h"
#include "shape.h"

struct World
{
    std::vector<std::unique_ptr<Light>> lights{};
    std::vector<std::unique_ptr<Shape>> shapes{};

    [[nodiscard]] Shape* getShapePtrFromId(int id) const {
        for(const auto& shape_ptr: shapes) {
            if (shape_ptr->id == id) {
                return shape_ptr.get();
            }
        }

        return nullptr;
    }

    template<class LightType>
    void addLight(const LightType& light) {
        lights.emplace_back(std::make_unique<LightType>(light));
    }

    template<class ShapeType>
    void addShape(const ShapeType& shape) {
        shapes.emplace_back(std::make_unique<ShapeType>(shape));
    }

};

#endif //WORLD_H
