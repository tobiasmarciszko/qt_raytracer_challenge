#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>
#include "matrix.h"

struct Camera {

    Camera() = delete;

    Camera(
        double hsize_,
        double vsize_,
        double field_of_view_) :
        hsize(hsize_),
        vsize(vsize_),
        field_of_view(field_of_view_) {

        const double half_view = std::tan(field_of_view_ / 2.0);
        const double aspect = hsize_ / vsize_;

        if (aspect >= 1) {
            half_width = half_view;
            half_height = half_view / aspect;
        } else {
            half_width = half_view * aspect;
            half_height = half_view;
        }

        pixel_size = (half_width * 2) / hsize;
    }

    double hsize; // In pixels
    double vsize;

    double field_of_view;

    Matrix<4,4> transform = identity_matrix;

    double half_width;
    double half_height;
    double pixel_size;

};

#endif //CAMERA_H
