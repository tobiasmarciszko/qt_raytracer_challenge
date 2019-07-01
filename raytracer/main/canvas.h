#ifndef CANVAS_H
#define CANVAS_H

#include <QVector>
#include "color.h"

class Canvas
{
public:
    Canvas(unsigned int width, unsigned int height) : m_width(width), m_height(height) {
        m_pixels.resize(width * height);
    }

    Canvas() {
        fill(Color(0.2,0.2,0.2));
    }

    void write_pixel(unsigned int x, unsigned int y, Color color) {
        m_pixels[m_width * y + x] = color;
    }

    Color pixel_at(unsigned int x, unsigned int y) const {

        auto index = m_width * y + x;
        auto size = m_pixels.size();

        return m_pixels[m_width * y + x];
    }

    void fill(Color color) {
        for (unsigned int x = 0; x < m_width; ++x) {
            for (unsigned int y = 0; y < m_height; ++y) {
                color.x = x;
                color.y = y;
                 m_pixels[m_width * y + x] = color;
            }
        }
    }

    QVector<Color> m_pixels{};

private:
    unsigned int m_width;
    unsigned int m_height;

};

#endif // CANVAS_H
