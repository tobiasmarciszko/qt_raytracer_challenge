#include "color.h"

Color::Color(double red, double green, double blue):
    m_red(red),
    m_green(green),
    m_blue(blue)
{
}


Color Color::operator+(const Color& color) const {
    double r = m_red + color.m_red;
    double g = m_green + color.m_green;
    double b = m_blue + color.m_blue;

    return {r, g, b};
}

Color Color::operator-(const Color& color) const {
    double r = m_red - color.m_red;
    double g = m_green - color.m_green;
    double b = m_blue - color.m_blue;
    return {r, g, b};
}

Color Color::operator*(const double& multiplier) const {
    double r = m_red * multiplier;
    double g = m_green * multiplier;
    double b = m_blue * multiplier;
    return {r, g, b};
}

Color Color::operator/(const double& divider) const {
    double r = m_red / divider;
    double g = m_green / divider;
    double b = m_blue / divider;
    return {r, g, b};
}

// Hadamard product (Schur product)
Color Color::operator *(const Color& color) const {
    double r = m_red * color.m_red;
    double g = m_green * color.m_green;
    double b = m_blue * color.m_blue;

    return {r, g, b};
}
