#include "matrix4x4.h"

Matrix4x4::Matrix4x4() :
    m00(0),
    m01(0),
    m02(0),
    m03(0),
    m10(0),
    m11(0),
    m12(0),
    m13(0),
    m20(0),
    m21(0),
    m22(0),
    m23(0),
    m30(0),
    m31(0),
    m32(0),
    m33(0)
{
}

Matrix4x4::Matrix4x4(
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
      ) :
    m00(m00),
    m01(m01),
    m02(m02),
    m03(m03),
    m10(m10),
    m11(m11),
    m12(m12),
    m13(m13),
    m20(m20),
    m21(m21),
    m22(m22),
    m23(m23),
    m30(m30),
    m31(m31),
    m32(m32),
    m33(m33)
{
}
