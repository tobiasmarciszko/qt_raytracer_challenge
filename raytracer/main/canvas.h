#ifndef CANVAS_H
#define CANVAS_H

#include <QVector>
#include "color.h"

class Canvas
{
public:
    Canvas() = delete;

    Canvas(unsigned int width, unsigned int height) : m_width(width), m_height(height) {
        m_pixels.resize(width * height);
        fill(Color(1, 1, 1));
    }

    void write_pixel(unsigned int x, unsigned int y, const Color& color) {
        m_pixels[m_width * y + x] = color;
    }

    Color pixel_at(unsigned int x, unsigned int y) const {
        return m_pixels[m_width * y + x];
    }

    void fill(const Color& color) {
        for (unsigned int x = 0; x < m_width; ++x) {
            for (unsigned int y = 0; y < m_height; ++y) {
                Color c = Color(color.red, color.green, color.blue);
                c.x = x;
                c.y = y;
                m_pixels[m_width * y + x] = c;
            }
        }
    }

    QVector<Color> m_pixels;

private:
    unsigned int m_width;
    unsigned int m_height;

};

#endif // CANVAS_H
