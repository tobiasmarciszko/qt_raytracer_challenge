#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include "tuple.h"

struct Color
{
public:
    Color() : red{0}, green{0}, blue{0} {}
    Color(double red, double green, double blue);

    Color operator +(const Color& color) const;
    Color operator -(const Color& color) const;
    Color operator *(const double& multiplier) const;
    Color operator *(const Color& color) const;
    Color operator /(const double& divider) const;
    bool operator ==(const Color& color) const;

    friend std::ostream &operator<<(std::ostream &os, const Color& c) {
        return os << "(" << c.red << ", " << c.green << ", " << c.blue << ")";
    }

    double red;
    double green;
    double blue;

    int x;
    int y;
};

#endif // COLOR_H
