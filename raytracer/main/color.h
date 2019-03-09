#ifndef COLOR_H
#define COLOR_H

#include "tuple.h"

class Color
{
public:
    Color(double red, double green, double blue);

    inline double red() { return m_red; }
    inline double green() { return m_green; }
    inline double blue() { return m_blue; }

    Color operator +(const Color& color) const;
    Color operator -(const Color& color) const;
    Color operator *(const double& multiplier) const;
    Color operator *(const Color& color) const;
    Color operator /(const double& divider) const;

private:
    double m_red;
    double m_green;
    double m_blue;

};

#endif // COLOR_H
