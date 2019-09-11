#include "catch.hpp"
#include "material.h"
#include "vector.h"
#include "lighting.h"
#include "sphere.h"
#include "shape.h"
#include "plane.h"
#include "math.h"
#include "computations.h"

TEST_CASE("Reflectivity for the default material")
{
    Material m;
    REQUIRE(equal(m.reflective, 0.0f));
}

TEST_CASE("Precomputing the reflection vector")
{
    std::shared_ptr<Shape> shape = std::make_shared<Plane>(Plane());
    Ray r(Point(0, 1, -1), Vector(0, -M_SQRT2/2, M_SQRT2/2));
    Intersection i(M_SQRT2, shape);
    Computations comps = prepare_computations(i, r);
    REQUIRE(comps.reflectv == Vector(0, M_SQRT2/2, M_SQRT2/2));
}
