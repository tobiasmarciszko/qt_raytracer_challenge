#include "color.h"

Color::Color(double red, double green, double blue):
    red(red),
    green(green),
    blue(blue)
{
}

Color Color::operator+(const Color& color) const {
    const double r = red + color.red;
    const double g = green + color.green;
    const double b = blue + color.blue;

    return {r, g, b};
}

Color Color::operator-(const Color& color) const {
    const double r = red - color.red;
    const double g = green - color.green;
    const double b = blue - color.blue;
    return {r, g, b};
}

Color Color::operator*(const double& multiplier) const {
    const double r = red * multiplier;
    const double g = green * multiplier;
    const double b = blue * multiplier;
    return {r, g, b};
}

Color Color::operator/(const double& divider) const {
    const double r = red / divider;
    const double g = green / divider;
    const double b = blue / divider;
    return {r, g, b};
}

// Hadamard product (Schur product)
Color Color::operator *(const Color& color) const {
    const double r = red * color.red;
    const double g = green * color.green;
    const double b = blue * color.blue;

    return {r, g, b};
}

bool Color::operator ==(const Color& color) const {
    if (!equal(red, color.red)) return false;
    if (!equal(green, color.green)) return false;
    if (!equal(blue, color.blue)) return false;

    return true;
}
