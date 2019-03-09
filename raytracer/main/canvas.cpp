#include "canvas.h"

Canvas::Canvas(unsigned int width, unsigned int height):
    m_width(width),
    m_height(height)
{
    m_pixels = new Color*[m_width];

    for (unsigned int i = 0; i < m_width; ++i) {
        m_pixels[i] = new Color[m_height];
    }

    for (unsigned int i = 0; i < m_width; ++i) {
        for (unsigned int j = 0; j < m_height; ++j) {
            m_pixels[i][j] = Color(0.2,0.2,0.2);
        }
    }
}

void Canvas::write_pixel(unsigned int x, unsigned int y, const Color& color) {
    m_pixels[x][y] = color;
}

Color Canvas::pixel_at(unsigned int x, unsigned int y) const {
    return m_pixels[x][y];
}
