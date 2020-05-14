#ifndef LIGHT_H
#define LIGHT_H

#include "point.h"
#include "vector.h"
#include "color.h"

struct Light
{
    Light(const Point& position, const Color& intensity) :
        position(position),
        intensity(intensity)
    {}

    [[nodiscard]] bool operator==(const Light& l) const {
        if (!(l.position == position)) return false;

        return (l.intensity == intensity);
    }

    Point position{0, 0, 0};
    Color intensity{1, 1, 1};
};

struct PointLight : public Light
{
    PointLight(
        const Point& pos,
        const Color& intensity) : Light(pos, intensity)
    {}
};

struct AreaLight : public Light
{
    AreaLight(
        const Point& corner,
        const Vector& full_uvec,
        const float usteps,
        const Vector& full_vvec,
        const float vsteps,
        const Color& intensity
        ) : Light(Point{1, 0, 0.5}, intensity),
            corner(corner),
            uvec(full_uvec / usteps),
            usteps(usteps),
            vvec(full_vvec / vsteps),
            vsteps(vsteps),
            samples(usteps * vsteps) {}

    Point corner;
    Vector uvec;
    float usteps;
    Vector vvec;
    float vsteps;
    float samples;
};

#endif //LIGHT_H
