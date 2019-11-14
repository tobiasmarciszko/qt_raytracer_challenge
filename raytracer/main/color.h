#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include "tuple.h"

struct Color
{
    Color() = default;
    Color(float r, float g, float b);

    Color operator +(const Color& color) const;
    Color operator -(const Color& color) const;
    Color operator *(const float& multiplier) const;
    Color operator *(const Color& color) const;
    Color operator /(const float& divider) const;
    bool operator ==(const Color& color) const;

    friend std::ostream &operator<<(std::ostream &os, const Color& c) {
        return os << "(" << c.red << ", " << c.green << ", " << c.blue << ")";
    }

    float red{0};
    float green{0};
    float blue{0};
};

struct Pixel
{
    Color color;
    int x{0};
    int y{0};
};

#endif // COLOR_H
