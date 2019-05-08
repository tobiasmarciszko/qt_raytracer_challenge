#ifndef CANVAS_H
#define CANVAS_H

#include <stdlib.h>
#include "color.h"

template<size_t width, size_t height>
class Canvas
{
public:
    Canvas() {
        for (unsigned int i = 0; i < m_width; ++i) {
            for (unsigned int j = 0; j < m_height; ++j) {
                m_pixels[i][j] = Color(0.2,0.2,0.2);
            }
        }
    }

    void write_pixel(unsigned int x, unsigned int y, const Color& color) {
        m_pixels[x][y] = color;
    }

    Color pixel_at(unsigned int x, unsigned int y) const {
        return m_pixels[x][y];
    }

private:
    size_t m_width = width;
    size_t m_height = height;

    Color m_pixels[width][height];
};

#endif // CANVAS_H
