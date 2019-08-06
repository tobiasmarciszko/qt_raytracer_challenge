#ifndef RAY_H
#define RAY_H

#include "point.h"
#include "vector.h"
#include "matrix.h"
#include "camera.h"
#include "point.h"

class Ray
{
public:
    Ray(const Point& origin, const Vector& direction) :
        m_origin(origin),
        m_direction(direction) {}

    inline Point origin() const {
        return m_origin;
    }

    inline Vector direction() const {
        return m_direction;
    }

    inline Point position(const double t) const {
        return m_origin + m_direction * t;
    }

    inline Ray transform(const Matrix<4,4>& matrix) const {
        return {matrix * m_origin, matrix * m_direction};
    }

private:
    Point m_origin;
    Vector m_direction;
};

inline Ray ray_for_pixel(const Camera& camera, unsigned int px, unsigned int py) {

    // the offset from the edge of the canvas to the pixel's center
    const auto xoffset = (px + 0.5) * camera.pixel_size;
    const auto yoffset = (py + 0.5) * camera.pixel_size;

    // px = (xoffset / camera.pixel_size) - 0.5;
    // py = (yoffset / camera.pixel_size) - 0.5;

    // the untransformed coordinates of the pixel in world space.
    //(remember that the camera looks toward -z, so +x is to the *left*.)
    const auto world_x = camera.half_width - xoffset;
    const auto world_y = camera.half_height - yoffset;

    // xoffset = camera.half_width - world_x;
    // yoffset = camera.half_height - world_y;

    // using the camera matrix, transform the canvas point and the origin,
    // and then compute the ray's direction vector.
    // (remember that the canvas is at z=-1)

    // const auto c_transform_inverse = camera.inverse_transform;

    // Point = pixel * camera.transform

    const auto pixel = camera.inverse_transform * Point(world_x, world_y, -1);
    const auto origin = camera.inverse_transform * Point(0, 0, 0);
    const auto direction = Vector(pixel - origin).normalize();

    return {origin, direction};
}



#endif // RAY_H
