#ifndef MATRIX4X4_H
#define MATRIX4X4_H


class Matrix4x4
{
public:

    Matrix4x4();
    Matrix4x4(
            double m00,
            double m01,
            double m02,
            double m03,
            double m10,
            double m11,
            double m12,
            double m13,
            double m20,
            double m21,
            double m22,
            double m23,
            double m30,
            double m31,
            double m32,
            double m33
          );

public:
    // Row 0
    double m00;
    double m01;
    double m02;
    double m03;

    // Row 1
    double m10;
    double m11;
    double m12;
    double m13;

    // Row 2
    double m20;
    double m21;
    double m22;
    double m23;

    // Row3
    double m30;
    double m31;
    double m32;
    double m33;
};

#endif // MATRIX4X4_H
