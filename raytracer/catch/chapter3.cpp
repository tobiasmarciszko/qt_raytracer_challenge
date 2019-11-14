#include "catch.hpp"
#include "tuple.h"
#include "point.h"
#include "tuple.h"
#include "point.h"
#include "vector.h"
#include "color.h"
#include "canvas.h"
#include "matrix.h"
#include "equal.h"

TEST_CASE("testMatrix4x4") {
    const auto m = Matrix<4,4>(
        1,    2,    3,    4,
        5.5,  6.5,  7.5,  8.5,
        9,    10,   11,   12,
        13.5, 14.5, 15.5, 16.5);

    REQUIRE(equal(1,    m.get(0, 0)));
    REQUIRE(equal(4,    m.get(0, 3)));
    REQUIRE(equal(5.5,  m.get(1, 0)));
    REQUIRE(equal(7.5,  m.get(1, 2)));
    REQUIRE(equal(11,   m.get(2, 2)));
    REQUIRE(equal(13.5, m.get(3, 0)));
    REQUIRE(equal(15.5, m.get(3, 2)));
}

TEST_CASE("testMatrix3x3") {
    const auto m = Matrix<3,3>(-3,    5,   0,
                               1,   -2,  -7,
                               0,    1,   1);

    REQUIRE(equal(-3, m.get(0, 0)));
    REQUIRE(equal(-2, m.get(1, 1)));
    REQUIRE(equal(1,  m.get(2, 2)));
}

TEST_CASE("testMatrix2x2") {
    const auto m = Matrix<2,2>(-3,  5,
                               1, -2);

    REQUIRE(equal(-3, m.get(0, 0)));
    REQUIRE(equal(5,  m.get(0, 1)));
    REQUIRE(equal(1,  m.get(1, 0)));
    REQUIRE(equal(-2,  m.get(1, 1)));
}

TEST_CASE("testMatrixEqual") {
    const auto a = Matrix<4,4>(
        1,2,3,4,
        5,6,7,8,
        9,8,7,6,
        5,4,3,2);

    const auto b = Matrix<4,4>(
        1,2,3,4,
        5,6,7,8,
        9,8,7,6,
        5,4,3,2);

    REQUIRE(a == b);

}

TEST_CASE("testMatrixNotEqual") {
    const auto a = Matrix<4,4>(
        1,2,3,4,
        5,6,7,8,
        9,8,7,6,
        5,4,3,2);

    const auto b = Matrix<4,4>(
        2,3,4,5,
        6,7,8,9,
        8,7,6,5,
        4,3,2,1);

    REQUIRE(a != b);
}

TEST_CASE("testMatrixMultiplication") {
    const auto a = Matrix<4,4>(
        1,2,3,4,
        5,6,7,8,
        9,8,7,6,
        5,4,3,2);

    const auto b = Matrix<4,4>(
        -2,1,2,3,
        3,2,1,-1,
        4,3,6,5,
        1,2,7,8);

    const auto expected = Matrix<4,4>(
        20,22,50,48,
        44,54,114,108,
        40,58,110,102,
        16,26,46,42);

    const auto result = a * b;

    REQUIRE(result == expected);

}

TEST_CASE("testMatrixTupleMultiplication") {
    const auto a = Matrix<4,4>(
        1,2,3,4,
        2,4,4,2,
        8,6,4,1,
        0,0,0,1);

    const auto b = Tuple(1, 2, 3, 1);

    const auto expected = Tuple(18, 24, 33, 1);

    const auto result = a * b;

    REQUIRE(result == expected);
}

TEST_CASE("testMultiplicationWithIdentityMatrix") {

    const auto A = Matrix<4,4>(
        0,1,2,4,
        1,2,4,8,
        2,4,8,16,
        4,8,16,32);

    REQUIRE((A *identity_matrix) == A);

    const auto a = Tuple(1, 2, 3, 4);

    REQUIRE((identity_matrix * a) == a);
}

TEST_CASE("testMatrixtranspose") {

    const auto A = Matrix<4,4>(
        0,9,3,0,
        9,8,0,8,
        1,8,5,3,
        0,0,5,8);

    const auto expected = Matrix<4,4>(
        0,9,1,0,
        9,8,8,0,
        3,0,5,5,
        0,8,3,8);

    const auto result = A.transpose();

    REQUIRE(result == expected);
}

TEST_CASE("testMatrixTransposeIdentity")
{
    // Transposing the identity matrix should be the... identity matrix! :)

    const auto result = identity_matrix.transpose();
    REQUIRE(identity_matrix == result);
}

TEST_CASE("testMatrix2x2determinant")
{
    const auto A = Matrix<2,2>(
        1, 5,
        -3, 2);

    const auto expected = 17;
    const auto result = A.determinant();

    REQUIRE(equal(result, expected));
}

TEST_CASE("test2x2SubMatrix")
{
    const auto A = Matrix<3,3>(
        1,5,0,
        -3,2,7,
        0,6,-3
    );

    const auto expected = Matrix<2,2>(
        -3,2,
        0,6);

    const auto result = A.submatrix(0,2);

    REQUIRE(expected == result);
}

TEST_CASE("test3x3SubMatrix")
{
    const auto A = Matrix<4,4>(
        -6,1,1,6,
        -8,5,8,6,
        -1,0,8,2,
        -7,1,-1,1);

    const auto expected = Matrix<3,3>(
        -6,1,6,
        -8,8,6,
        -7,-1,1);

    const auto result = A.submatrix(2,1);

    REQUIRE(expected == result);
}

TEST_CASE("testMinor")
{
    const auto A = Matrix<3,3>(
        3,5,0,
        2,-1,-7,
        6,-1,5);

    const auto B = A.submatrix(1,0);

    const auto expected = 25;

    const auto determinant = B.determinant();
    const auto minor = A.minor(1,0);

    REQUIRE(equal(expected, determinant));
    REQUIRE(equal(expected, minor));
}

TEST_CASE("testCofactor")
{
    const auto A = Matrix<3,3>(
        3,5,0,
        2,-1,-7,
        6,-1,5);

    REQUIRE(equal(A.minor(0,0), -12));
    REQUIRE(equal(A.cofactor(0,0), -12));
    REQUIRE(equal(A.minor(1,0), 25));
    REQUIRE(equal(A.cofactor(1,0), -25));
}

TEST_CASE("testMatrix3x3determinant")
{
    const auto A = Matrix<3,3>(
        1,2,6,
        -5,8,-4,
        2,6,4);

    REQUIRE(equal(A.cofactor(0,0), 56));
    REQUIRE(equal(A.cofactor(0,1), 12));
    REQUIRE(equal(A.cofactor(0,2), -46));
    REQUIRE(equal(A.determinant(), -196));
}

TEST_CASE("testMatrix4x4determinant")
{
    const auto A = Matrix<4,4>(
        -2,-8,3,5,
        -3,1,7,3,
        1,2,-9,6,
        -6,7,7,-9);

    REQUIRE(equal(A.cofactor(0,0), 690));
    REQUIRE(equal(A.cofactor(0,1), 447));
    REQUIRE(equal(A.cofactor(0,2), 210));
    REQUIRE(equal(A.cofactor(0,3), 51));
    REQUIRE(equal(A.determinant(), -4071));
}

TEST_CASE("testInvertible")
{
    const auto A = Matrix<4,4>(
        6,4,4,4,
        5,5,7,6,
        4,-9,3,-7,
        9,1,7,-6);

    REQUIRE(equal(A.determinant(), -2120));
    REQUIRE(A.invertible());
}

TEST_CASE("testNonInvertible")
{
    const auto A = Matrix<4,4>(
        -4,2,-2,-3,
        9,6,2,6,
        0,-5,1,-5,
        0,0,0,0);

    REQUIRE(equal(A.determinant(), 0));
    REQUIRE(A.invertible() == false);
}

TEST_CASE("testInverse")
{
    const auto A = Matrix<4,4>(
        -5,2,6,-8,
        1,-5,1,8,
        7,7,-6,-7,
        1,-3,7,4);

    const auto B = A.inverse();

    const auto expected = Matrix<4,4>(
        0.21805,  0.45113,  0.24060, -0.04511,
        -0.80827, -1.45677, -0.44361, 0.52068,
        -0.07895, -0.22368, -0.05263, 0.19737,
        -0.52256, -0.81391, -0.30075, 0.30639
    );

    REQUIRE(equal(A.determinant(), 532.0));
    REQUIRE(equal(A.cofactor(2,3), -160.0));
    REQUIRE(equal(B.get(3,2), -160.0/532.0));
    REQUIRE(equal(A.cofactor(3,2), 105.0));
    REQUIRE(equal(B.get(2,3), 105.0/532.0));

    REQUIRE(B == expected);
}

TEST_CASE("testInverseAnother")
{
    const auto A = Matrix<4,4>(
        8 , -5,  9,  2,
        7 ,  5,  6,  1,
        -6 ,  0,  9,  6,
        -3 ,  0, -9, -4);

    const auto expected = Matrix<4,4> (
        -0.15385, -0.15385, -0.28205, -0.53846,
        -0.07692,  0.12308,  0.02564,  0.03077,
        0.35897,  0.35897,  0.43590,  0.92308,
        -0.69231, -0.69231, -0.76923, -1.92308);

    REQUIRE(A.inverse() == expected);
}

TEST_CASE("testInverseThird")
{
    const auto A = Matrix<4,4>(
        9,  3,  0,  9,
        -5, -2, -6, -3,
        -4,  9,  6,  4,
        -7,  6,  6,  2);

    const auto expected = Matrix<4,4> (
        -0.04074, -0.07778,  0.14444, -0.22222,
        -0.07778,  0.03333,  0.36667, -0.33333,
        -0.02901, -0.14630, -0.10926,  0.12963,
        0.17778,  0.06667, -0.26667,  0.33333);

    REQUIRE(A.inverse() == expected);
}

TEST_CASE("testInverseMultiply")
{
    const auto A = Matrix<4,4>(
        3, -9,  7,  3,
        3, -8,  2, -9,
        -4,  4,  4,  1,
        -6,  5, -1,  1);

    const auto B = Matrix<4,4>(
        8,  2, 2, 2,
        3, -1, 7, 0,
        7,  0, 5, 4,
        6, -2, 0, 5);

    const auto C = A * B;

    REQUIRE((C * B.inverse()) == A);
}
