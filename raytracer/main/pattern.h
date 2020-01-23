#ifndef PATTERN_H
#define PATTERN_H

#include "color.h"
#include "point.h"
#include "matrix.h"

// Predefined colors for convenience
const Color black = Color(0,0,0);
const Color white = Color(1,1,1);

struct Pattern {
    virtual ~Pattern() = default;

    Matrix<4,4> transform = identity_matrix;
    Matrix<4,4> inverse_transform = transform.inverse();

    [[nodiscard]] virtual Color pattern_at(const Point& p) const = 0;

    inline void set_transform(const Matrix<4,4>& t) {
        transform = t;
        inverse_transform = transform.inverse();
    }
};

#endif // PATTERN_H
