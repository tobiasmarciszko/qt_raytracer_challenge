#ifndef LIGHT_H
#define LIGHT_H

#include "point.h"
#include "color.h"

class Light
{
public:
    Light(const Point& pos, const Color& intensity) :
        m_position(pos),
        m_intensity(intensity)
    {}

    inline auto intensity() const {
        return m_intensity;
    }

    inline auto position() const {
        return m_position;
    }

private:
    Point m_position = Point(0, 0, 0);
    Color m_intensity = Color(1, 1, 1);

};

class PointLight : public Light
{
public:
    PointLight(
        const Point& pos,
        const Color& intensity) : Light(pos, intensity)
    {}
};

#endif //LIGHT_H
