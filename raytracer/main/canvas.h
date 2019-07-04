#ifndef CANVAS_H
#define CANVAS_H

#include <QVector>
#include "color.h"

class Canvas
{
public:
    Canvas() = delete;

    Canvas(int width, int height) : m_width(width), m_height(height) {
        pixels.resize(width * height);
        fill(Color(1, 1, 1));
    }

    void write_pixel(int x, int y, const Color& color) {
        Pixel pixel;
        pixel.color = color;
        pixel.x = x;
        pixel.y = y;
        pixels[m_width * y + x] = pixel;
    }

    Pixel pixel_at(int x, int y) const {
        return pixels[m_width * y + x];
    }

    void fill(const Color& color) {
        for (int x = 0; x < m_width; ++x) {
            for (int y = 0; y < m_height; ++y) {
                write_pixel(x, y, color);
            }
        }
    }

    QVector<Pixel> pixels;

private:
    int m_width;
    int m_height;
};

#endif // CANVAS_H
