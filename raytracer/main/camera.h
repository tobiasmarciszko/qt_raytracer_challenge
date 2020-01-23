#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>
#include "matrix.h"

struct Camera {

    Camera() = default;

    Camera(
        float hsize_,
        float vsize_,
        float field_of_view_) :
        hsize(hsize_),
        vsize(vsize_),
        field_of_view(field_of_view_) {

        const float half_view = std::tan(field_of_view_ / 2.0F);
        const float aspect = hsize_ / vsize_;

        if (aspect >= 1.0) {
            half_width = half_view;
            half_height = half_view / aspect;
        } else {
            half_width = half_view * aspect;
            half_height = half_view;
        }

        pixel_size = (half_width * 2.0F) / hsize;
    }


    inline void set_transform(const Matrix<4,4>& transform) {
        m_transform = transform;
        m_inverse_transform = transform.inverse();
    }

    [[nodiscard]] inline auto transform() const {
        return m_transform;
    }

    [[nodiscard]] inline auto inverse_transform() const {
        return m_inverse_transform;
    }

    float hsize{}; // In pixels
    float vsize{};

    float field_of_view{};

    float half_width{};
    float half_height{};
    float pixel_size{};

private:
    Matrix<4,4> m_transform{identity_matrix};
    Matrix<4,4> m_inverse_transform{identity_matrix};
};

#endif //CAMERA_H
