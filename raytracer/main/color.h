#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include "tuple.h"

struct Color
{
    Color() : red{0}, green{0}, blue{0} {}
    Color(float red, float green, float blue);

    Color operator +(const Color& color) const;
    Color operator -(const Color& color) const;
    Color operator *(const float& multiplier) const;
    Color operator *(const Color& color) const;
    Color operator /(const float& divider) const;
    bool operator ==(const Color& color) const;

    friend std::ostream &operator<<(std::ostream &os, const Color& c) {
        return os << "(" << c.red << ", " << c.green << ", " << c.blue << ")";
    }

    float red;
    float green;
    float blue;
};

struct Pixel
{
    Color color;
    int x;
    int y;
};

#endif // COLOR_H
