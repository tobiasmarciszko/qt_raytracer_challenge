#ifndef CANVAS_H
#define CANVAS_H

#include "color.h"

class Canvas
{
public:
    Canvas(unsigned int width, unsigned int height);
    ~Canvas();

    void write_pixel(unsigned int x, unsigned int y, const Color& color);
    Color pixel_at(unsigned int x, unsigned int y) const;

private:
    unsigned int m_width;
    unsigned int m_height;

    Color **m_pixels;
};

#endif // CANVAS_H
