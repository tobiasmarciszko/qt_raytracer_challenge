#include "color.h"

Color::Color(float red, float green, float blue):
    red(red),
    green(green),
    blue(blue)
{
}

Color Color::operator+(const Color& color) const {
    const float r = red + color.red;
    const float g = green + color.green;
    const float b = blue + color.blue;

    return {r, g, b};
}

Color Color::operator-(const Color& color) const {
    const float r = red - color.red;
    const float g = green - color.green;
    const float b = blue - color.blue;
    return {r, g, b};
}

Color Color::operator*(const float& multiplier) const {
    const float r = red * multiplier;
    const float g = green * multiplier;
    const float b = blue * multiplier;
    return {r, g, b};
}

Color Color::operator/(const float& divider) const {
    const float r = red / divider;
    const float g = green / divider;
    const float b = blue / divider;
    return {r, g, b};
}

// Hadamard product (Schur product)
Color Color::operator *(const Color& color) const {
    const float r = red * color.red;
    const float g = green * color.green;
    const float b = blue * color.blue;

    return {r, g, b};
}

bool Color::operator ==(const Color& color) const {
    if (!equal(red, color.red)) return false;
    if (!equal(green, color.green)) return false;
    if (!equal(blue, color.blue)) return false;

    return true;
}
