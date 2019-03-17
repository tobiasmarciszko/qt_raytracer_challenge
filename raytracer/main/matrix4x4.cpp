#include "matrix4x4.h"

Matrix4x4::Matrix4x4()
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
      )

{
    m_data[0][0] = m00;
    m_data[0][1] = m01;
    m_data[0][2] = m02;
    m_data[0][3] = m03;

    m_data[1][0] = m10;
    m_data[1][1] = m11;
    m_data[1][2] = m12;
    m_data[1][3] = m13;

    m_data[2][0] = m20;
    m_data[2][1] = m21;
    m_data[2][2] = m22;
    m_data[2][3] = m23;

    m_data[3][0] = m30;
    m_data[3][1] = m31;
    m_data[3][2] = m32;
    m_data[3][3] = m33;
}


