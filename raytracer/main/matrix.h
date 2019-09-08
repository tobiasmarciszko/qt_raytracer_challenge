#ifndef MATRIX_H
#define MATRIX_H

#include <cstdlib>
#include <cmath>
#include "tuple.h"

template <size_t rows, size_t cols>
class Matrix
{
public:

    // 2x2
    Matrix(
            const float m00,
            const float m01,
            const float m10,
            const float m11
          )
    {
        set(0, 0, m00);
        set(0, 1, m01);
        set(1, 0, m10);
        set(1, 1, m11);
    }

    // 3x3
    Matrix(
            const float m00,
            const float m01,
            const float m02,
            const float m10,
            const float m11,
            const float m12,
            const float m20,
            const float m21,
            const float m22
          )
    {
        set(0, 0, m00);
        set(0, 1, m01);
        set(0, 2, m02);
        set(1, 0, m10);
        set(1, 1, m11);
        set(1, 2, m12);
        set(2, 0, m20);
        set(2, 1, m21);
        set(2, 2, m22);
    }

    // 4x4
    Matrix(
            const float m00,
            const float m01,
            const float m02,
            const float m03,
            const float m10,
            const float m11,
            const float m12,
            const float m13,
            const float m20,
            const float m21,
            const float m22,
            const float m23,
            const float m30,
            const float m31,
            const float m32,
            const float m33
          )
    {
        set(0, 0, m00);
        set(0, 1, m01);
        set(0, 2, m02);
        set(0, 3, m03);
        set(1, 0, m10);
        set(1, 1, m11);
        set(1, 2, m12);
        set(1, 3, m13);
        set(2, 0, m20);
        set(2, 1, m21);
        set(2, 2, m22);
        set(2, 3, m23);
        set(3, 0, m30);
        set(3, 1, m31);
        set(3, 2, m32);
        set(3, 3, m33);
    }

    Matrix<rows,cols>() = default;

    inline void set(const int row, const int col, const float value) { m_data[cols * row + col] = value; }
    constexpr inline float get(const unsigned int row, const unsigned int col) const { return m_data[cols * row + col]; }
    inline size_t getRowCount() const { return m_rows; }
    inline size_t getColCount() const { return m_cols; }

    inline bool operator==(const Matrix<rows, cols>& matrix) const {

        if (m_rows != matrix.getRowCount()) return false;
        if (m_cols != matrix.getColCount()) return false;

        for (size_t i = 0; i < m_rows; i++) {
            for (size_t j = 0; j < m_cols; j++) {
                if (!equal(get(i, j), matrix.get(i, j))) return false;
            }
        }

        return true;
    }

    inline bool operator!=(const Matrix<rows, cols>& matrix) const {
        return !(*this == matrix);
    }

    inline Matrix<4,4> operator*(const Matrix<4,4>& multiplier) const;
    inline Tuple operator*(const Tuple& tuple) const;

    inline Matrix<rows, cols> transpose() const {
        Matrix<rows, cols> result;

        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                const int row = static_cast<unsigned int>(i);
                const int col = static_cast<unsigned int>(j);

                result.set(col, row, get(row, col));
            }
        }

        return result;
    }

    inline float determinant() const {
        float det = 0;
        for (size_t i = 0; i < cols; i++) {
            const int col = static_cast<int>(i);
            det += get(0, col) * cofactor(0, col);
        }

        return det;
    }

    inline Matrix<rows-1, cols-1> submatrix(const unsigned int rowToRemove, const unsigned int colToRemove) const {
        Matrix<rows-1, cols-1> result;

        unsigned int sourceRow = 0;
        for (size_t i = 0; i < rows-1; i++) {
            const auto row = static_cast<unsigned int>(i);

            // skip this row
            if (row == rowToRemove) { sourceRow++; }

            unsigned int sourceCol = 0;
            for (size_t j = 0; j < cols-1; j++) {
                const auto col = static_cast<unsigned int>(j);

                // skip this col
                if (col == colToRemove) { sourceCol++; }

                result.set(row, col, get(sourceRow, sourceCol));
                sourceCol++;
            }
            sourceRow++;
        }
        return result;
    }

    #undef minor
    inline float minor(const unsigned int row, const unsigned int column) const {
        const auto sub = submatrix(row, column);
        return sub.determinant();
    }

    inline float cofactor(const unsigned int row, const unsigned int column) const {
        auto minorValue = minor(row, column);

        // Cofactor is the same as the minor expected:
        // If row+column is an odd number, negate!

        if ((row + column) % 2 != 0) {
            minorValue *= -1;
        }

        return minorValue;
    }

    inline bool invertible() const {
        return determinant() != 0;
    }

    inline Matrix<rows, cols> inverse() const {
        Matrix<rows, cols> result;
        const auto det = determinant();

        for (unsigned int row = 0; row < rows; row++) {
            for (unsigned int col = 0; col < cols; col++) {
                const auto c = cofactor(row, col);
                result.set(col, row, c / det);
            }
        }

        return result;
    }

public:

    size_t m_rows = rows;
    size_t m_cols = cols;

    float m_data[rows * cols]{0.0};

};

// 2x2 specializations
template<>
inline float Matrix<2, 2>::determinant() const {
    // [a b]
    // [c d]
    //
    // determinant = a*d - b*c

    const auto ad = get(0, 0) * get(1, 1);
    const auto bc = get(0, 1) * get(1, 0);

    const auto result = ad - bc;

    return result;
}

// 4x4 specializations
template<>
inline Tuple Matrix<4, 4>::operator*(const Tuple& tuple) const {
    float result[4];
    for (unsigned int i = 0; i < 4; i++) {
            result[i] = get(i, 0) * tuple.x +
                        get(i, 1) * tuple.y +
                        get(i, 2) * tuple.z +
                        get(i, 3) * tuple.w;
        }

    return {result[0], result[1], result[2], result[3]};
}

template<>
inline Matrix<4, 4> Matrix<4,4>::operator*(const Matrix<4, 4>& multiplier) const {
    Matrix<4,4> result;
    for (unsigned int row = 0; row < 4; row++) {
        for (unsigned int col = 0; col < 4; col++) {
            auto value = get(row, 0) * multiplier.get(0, col) +
                         get(row, 1) * multiplier.get(1, col) +
                         get(row, 2) * multiplier.get(2, col) +
                         get(row, 3) * multiplier.get(3, col);

            result.set(row, col, value);
        }
    }
    return result;
}

const auto identity_matrix = Matrix<4,4>(
                                        1,0,0,0,
                                        0,1,0,0,
                                        0,0,1,0,
                                        0,0,0,1);

// Transformation matrices
inline Matrix<4,4> translation(const float x,
                               const float y,
                               const float z) {
    return {
            1,0,0,x,
            0,1,0,y,
            0,0,1,z,
            0,0,0,1
    };

}

inline Matrix<4,4> scaling(const float x,
                           const float y,
                           const float z) {
    return {
            x,0,0,0,
            0,y,0,0,
            0,0,z,0,
            0,0,0,1
    };
}

inline Matrix<4,4> rotation_x(const float r) {
    return {
            1, 0, 0, 0,
            0, std::cos(r), -std::sin(r), 0,
            0, std::sin(r), std::cos(r), 0,
            0, 0, 0, 1
    };
}

inline Matrix<4,4> rotation_y(const float r) {
    return {
            std::cos(r), 0, std::sin(r), 0,
            0, 1, 0, 0,
            -std::sin(r), 0, std::cos(r), 0,
            0, 0, 0, 1
    };
}

inline Matrix<4,4> rotation_z(const float r) {
    return {
            std::cos(r), -std::sin(r), 0, 0,
            std::sin(r), std::cos(r), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
    };
}

inline Matrix<4,4> shearing(const float xy, const float xz,
                            const float yx, const float yz,
                            const float zx, const float zy) {
    return {
            1,  xy, xz, 0,
            yx, 1,  yz, 0,
            zx, zy,  1, 0,
            0,  0,   0, 1
    };
}
#endif // MATRIX_H
