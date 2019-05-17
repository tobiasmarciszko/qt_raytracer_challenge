#include "catch.hpp"
#include "tuple.h"
#include "point.h"
#include "tuple.h"
#include "point.h"
#include "vector.h"
#include "color.h"
#include "canvas.h"
#include "matrix.h"
#include "ray.h"
#include "sphere.h"

TEST_CASE("Test Tuple") {
    Tuple tuple(1.0, 2.0, 3.0, 4.0);

    REQUIRE(equal(1.0, tuple.x()));
    REQUIRE(equal(2.0, tuple.y()));
    REQUIRE(equal(3.0, tuple.z()));
    REQUIRE(equal(4.0, tuple.w()));
}

TEST_CASE("testPoint")
{
    Point point = Point(1, 2, 3);
    REQUIRE(point.isPoint());

    Tuple tuple = point;
    REQUIRE(equal(1, tuple.w()));
}

TEST_CASE("testVector")
{
    Vector vector = Vector(1, 2, 3);
    REQUIRE(vector.isVector());

    Tuple tuple = vector;
    REQUIRE(equal(0, tuple.w()));
}

TEST_CASE("testAdd")
{
    Tuple a1 = Tuple(3, -2, 5, 1);
    Tuple a2 = Tuple(-2, 3, 1, 0);

    Tuple a3 = a1 + a2;

    REQUIRE(equal(1, a3.x()));
    REQUIRE(equal(1, a3.y()));
    REQUIRE(equal(6, a3.z()));
    REQUIRE(equal(1, a3.w()));
}

TEST_CASE("testSubtractPoints")
{
    Point p1 = Point(3, 2, 1);
    Point p2 = Point(5, 6, 7);

    Vector v1 = p1 - p2;

    REQUIRE(v1.isVector());
    REQUIRE(equal(-2, v1.x()));
    REQUIRE(equal(-4, v1.y()));
    REQUIRE(equal(-6, v1.z()));
}

TEST_CASE("testSubtractVectorFromPoint")
{
    Point p = Point(3, 2, 1);
    Vector v = Vector(5, 6, 7);

    Point p1 = p - v;

    REQUIRE(p1.isPoint());
    REQUIRE(equal(-2, p1.x()));
    REQUIRE(equal(-4, p1.y()));
    REQUIRE(equal(-6, p1.z()));
}

TEST_CASE("testSubtractVectors")
{
    Vector v1 = Vector(3, 2, 1);
    Vector v2 = Vector(5, 6, 7);

    Vector v3 = v1 - v2;

    REQUIRE(v3.isVector());
    REQUIRE(equal(-2, v3.x()));
    REQUIRE(equal(-4, v3.y()));
    REQUIRE(equal(-6, v3.z()));
}

TEST_CASE("testNegate")
{
    Tuple tuple = -Tuple(1, -2, 3, -4);

    REQUIRE(equal(-1, tuple.x()));
    REQUIRE(equal(2, tuple.y()));
    REQUIRE(equal(-3, tuple.z()));
    REQUIRE(equal(4, tuple.w()));
}

TEST_CASE("testScalarMultiply")
{
    Tuple a = Tuple(1, -2, 3, -4);
    Tuple tuple = a * 3.5;

    REQUIRE(equal(3.5, tuple.x()));
    REQUIRE(equal(-7, tuple.y()));
    REQUIRE(equal(10.5, tuple.z()));
    REQUIRE(equal(-14, tuple.w()));
}

TEST_CASE("testFractionMultiply")
{
    Tuple a = Tuple(1, -2, 3, -4);
    Tuple tuple = a * 0.5;

    REQUIRE(equal(0.5, tuple.x()));
    REQUIRE(equal(-1, tuple.y()));
    REQUIRE(equal(1.5, tuple.z()));
    REQUIRE(equal(-2, tuple.w()));
}

TEST_CASE("testScalarDivision")
{
    Tuple a = Tuple(1, -2, 3, -4);
    Tuple tuple = a / 2;

    REQUIRE(equal(0.5, tuple.x()));
    REQUIRE(equal(-1, tuple.y()));
    REQUIRE(equal(1.5, tuple.z()));
    REQUIRE(equal(-2, tuple.w()));
}

TEST_CASE("testMagnitude")
{
    Vector v1 = Vector(1,0,0);
    REQUIRE(equal(1, v1.magnitude()));

    Vector v2 = Vector(0,1,0);
    REQUIRE(equal(1, v2.magnitude()));

    Vector v3 = Vector(0,0,1);
    REQUIRE(equal(1, v3.magnitude()));

    Vector v4 = Vector(1,2,3);
    REQUIRE(equal(sqrt(14), v4.magnitude()));

    Vector v5 = Vector(-1,-2,-3);
    REQUIRE(equal(sqrt(14), v5.magnitude()));
}

TEST_CASE("testNormalize")
{
    Vector v1 = Vector(4,0,0);
    Vector v2 = v1.normalize();

    REQUIRE(equal(1, v2.x()));

    Vector v3 = Vector(1,2,3);
    Vector v4 = v3.normalize();

    REQUIRE(equal(1 / sqrt(14), v4.x()));
    REQUIRE(equal(2 / sqrt(14), v4.y()));
    REQUIRE(equal(3 / sqrt(14), v4.z()));
}

TEST_CASE("testDot")
{
    Vector a = Vector(1,2,3);
    Vector b = Vector(2,3,4);

    double dot = a.dot(b);

    REQUIRE(equal(20, dot));
}

TEST_CASE("testCross")
{
    Vector a = Vector(1,2,3);
    Vector b = Vector(2,3,4);

    Vector cross1 = a.cross(b);
    REQUIRE(equal(-1, cross1.x()));
    REQUIRE(equal(2, cross1.y()));
    REQUIRE(equal(-1, cross1.z()));

    Vector cross2 = b.cross(a);
    REQUIRE(equal(1, cross2.x()));
    REQUIRE(equal(-2, cross2.y()));
    REQUIRE(equal(1, cross2.z()));

}

// Chapter 2

TEST_CASE("testColor")
{
    Color c = Color(-0.5, 0.4, 1.7);

    REQUIRE(equal(-0.5, c.red()));
    REQUIRE(equal(0.4, c.green()));
    REQUIRE(equal(1.7, c.blue()));
}

TEST_CASE("testColorAdd")
{
    Color c1 = Color(0.9, 0.6, 0.75);
    Color c2 = Color(0.7, 0.1, 0.25);

    Color c3 = c1 + c2;

    REQUIRE(equal(1.6, c3.red()));
    REQUIRE(equal(0.7, c3.green()));
    REQUIRE(equal(1.0, c3.blue()));
}

TEST_CASE("testColorSubtract")
{
    Color c1 = Color(0.9, 0.6, 0.75);
    Color c2 = Color(0.7, 0.1, 0.25);

    Color c3 = c1 - c2;

    REQUIRE(equal(0.2, c3.red()));
    REQUIRE(equal(0.5, c3.green()));
    REQUIRE(equal(0.5, c3.blue()));
}

TEST_CASE("testColorScalar")
{
    Color c1 = Color(0.2, 0.3, 0.4);
    Color c2 = c1 * 2;

    REQUIRE(equal(0.4, c2.red()));
    REQUIRE(equal(0.6, c2.green()));
    REQUIRE(equal(0.8, c2.blue()));
}

TEST_CASE("testColorHadamard") {

    Color c1 = Color(1, 0.2, 0.4);
    Color c2 = Color(0.9, 1, 0.1);

    Color c3 = c1 * c2;

    REQUIRE(equal(0.9, c3.red()));
    REQUIRE(equal(0.2, c3.green()));
    REQUIRE(equal(0.04, c3.blue()));
}

TEST_CASE("testCanvas") {
    Canvas<10,20> c;
    Color red = Color(1, 0, 0);

    c.write_pixel(0, 0, red);
    REQUIRE(equal(1, c.pixel_at(0, 0).red()));

    c.write_pixel(10 - 1, 20 - 1, red);
    REQUIRE(equal(1, c.pixel_at(10 - 1, 20 - 1).red()));
}

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

TEST_CASE("testTranslation")
{
    const auto transform = translation(5, -3, 2);
    const auto p = Point(-3, 4, 5);

    const auto result = transform * p;

    REQUIRE(result == Point(2, 1, 7));
}

TEST_CASE("testInverseTranslation")
{
    const auto transform = translation(5, -3, 2);
    const auto inv = transform.inverse();
    const auto p = Point(-3, 4, 5);

    const auto result = inv * p;

    REQUIRE(result == Point(-8, 7, 3));
}

TEST_CASE("testTranslateVector")
{
    const auto transform = translation(5, -3, 2);
    const auto v = Vector(-3, 4, 5);

    const auto result = transform * v;

    REQUIRE(result == v);
}

TEST_CASE("testScalePoint")
{
    const auto transform = scaling(2, 3, 4);
    const auto p = Point(-4, 6, 8);

    const auto result = transform * p;

    REQUIRE(result == Point(-8, 18, 32));
}

TEST_CASE("testScaleVector")
{
    const auto transform = scaling(2, 3, 4);
    const auto v = Vector(-4, 6, 8);

    const auto result = transform * v;

    REQUIRE(result == Vector(-8, 18, 32));
}

TEST_CASE("testInverseScale")
{
    const auto transform = scaling(2, 3, 4);
    const auto inv = transform.inverse();
    const auto v = Vector(-4, 6, 8);

    const auto result = inv * v;

    REQUIRE(result == Vector(-2, 2, 2));
}

TEST_CASE("testReflection")
{
    const auto transform = scaling(-1, 1, 1);
    const auto p = Point(2, 3, 4);

    const auto result = transform * p;

    REQUIRE(result == Point(-2, 3, 4));
}

TEST_CASE("testRotateX")
{
    const auto p = Point(0, 1, 0);
    const auto half_quarter = rotation_x(M_PI_4);
    const auto full_quarter = rotation_x(M_PI_2);

    REQUIRE(half_quarter * p == Point(0, M_SQRT2 / 2, M_SQRT2 / 2));
    REQUIRE(full_quarter * p == Point(0, 0, 1));
}

TEST_CASE("testRotateInverseX")
{
    const auto p = Point(0, 1, 0);
    const auto half_quarter = rotation_x(M_PI_4);
    const auto inv = half_quarter.inverse();

    REQUIRE(inv * p == Point(0, M_SQRT2 / 2, - M_SQRT2 / 2));
}

TEST_CASE("testRotateY")
{
    const auto p = Point(0, 0, 1);
    const auto half_quarter = rotation_y(M_PI_4);
    const auto full_quarter = rotation_y(M_PI_2);

    REQUIRE(half_quarter * p == Point(M_SQRT2 / 2, 0, M_SQRT2 / 2));
    REQUIRE(full_quarter * p == Point(1, 0, 0));
}

TEST_CASE("testRotateInverseY")
{
    const auto p = Point(0, 0, 1);
    const auto half_quarter = rotation_y(M_PI_4);
    const auto inv = half_quarter.inverse();

    REQUIRE(inv * p == Point(-M_SQRT2 / 2, 0, M_SQRT2 / 2));
}


TEST_CASE("testRotateZ")
{
    const auto p = Point(0, 1, 0);
    const auto half_quarter = rotation_z(M_PI_4);
    const auto full_quarter = rotation_z(M_PI_2);

    REQUIRE(half_quarter * p == Point(-M_SQRT2 / 2, M_SQRT2 / 2, 0));
    REQUIRE(full_quarter * p == Point(-1, 0, 0));
}

TEST_CASE("testRotateInverseZ")
{
    const auto p = Point(0, 1, 0);
    const auto half_quarter = rotation_z(M_PI_4);
    const auto inv = half_quarter.inverse();

    REQUIRE(inv * p == Point(M_SQRT2 / 2, M_SQRT2 / 2, 0));
}

TEST_CASE("testShearingXY")
{
    auto const transform = shearing(1, 0, 0, 0, 0, 0);
    auto const p = Point(2, 3, 4);

    REQUIRE(transform * p == Point(5, 3, 4));
}

TEST_CASE("testShearingXZ")
{
    auto const transform = shearing(0, 1, 0, 0, 0, 0);
    auto const p = Point(2, 3, 4);

    REQUIRE(transform * p == Point(6, 3, 4));
}

TEST_CASE("testShearingYX")
{
    auto const transform = shearing(0, 0, 1, 0, 0, 0);
    auto const p = Point(2, 3, 4);

    REQUIRE(transform * p == Point(2, 5, 4));
}

TEST_CASE("testShearingYZ")
{
    auto const transform = shearing(0, 0, 0, 1, 0, 0);
    auto const p = Point(2, 3, 4);

    REQUIRE(transform * p == Point(2, 7, 4));
}

TEST_CASE("testShearingZX")
{
    auto const transform = shearing(0, 0, 0, 0, 1, 0);
    auto const p = Point(2, 3, 4);

    REQUIRE(transform * p == Point(2, 3, 6));
}

TEST_CASE("testShearingZY")
{
    auto const transform = shearing(0, 0, 0, 0, 0, 1);
    auto const p = Point(2, 3, 4);

    REQUIRE(transform * p == Point(2, 3, 7));
}

TEST_CASE("testTransformationSequence")
{
    const auto p = Point(1, 0, 1);
    const auto A = rotation_x(M_PI_2);
    const auto B = scaling(5, 5, 5);
    const auto C = translation(10, 5, 7);

    // apply rotation
    const auto p2 = A * p;
    REQUIRE(p2 == Point(1, -1, 0));

    // apply scaling
    const auto p3 = B * p2;
    REQUIRE(p3 == Point(5, -5, 0));

    // apply translation
    const auto p4 = C * p3;
    REQUIRE(p4 == Point(15, 0, 7));
}

TEST_CASE("testChainTransformationsInReverseOrder")
{
    const auto p = Point(1, 0, 1);
    const auto A = rotation_x(M_PI_2);
    const auto B = scaling(5, 5, 5);
    const auto C = translation(10, 5, 7);

    const auto T = C * B * A;
    REQUIRE((T * p) == Point(15, 0, 7));
}

TEST_CASE("testRayCreation")
{
    const auto origin = Point(1, 2, 3);
    const auto direction = Vector(4, 5, 6);

    const auto r = Ray(origin, direction);

    REQUIRE(r.origin() == origin);
    REQUIRE(r.direction() == direction);
}

TEST_CASE("testRayPosition")
{
    const auto r = Ray(Point(2, 3, 4), Vector(1, 0, 0));

    REQUIRE(r.position(0) == Point(2, 3, 4));
    REQUIRE(r.position(1) == Point(3, 3, 4));
    REQUIRE(r.position(-1) == Point(1, 3, 4));
    REQUIRE(r.position(2.5) == Point(4.5, 3, 4));
}

TEST_CASE("testRayIntersectsSphereAtTwoPoints")
{
    const auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
    const auto s = Sphere();

    const auto xs = s.intersect(r);

    REQUIRE(xs.size() == 2);
    REQUIRE(equal(xs.at(0).t(), 4.0));
    REQUIRE(equal(xs.at(1).t(), 6.0));
}

TEST_CASE("testRayIntersectsSphereAtTangent")
{
    const auto r = Ray(Point(0, 1, -5), Vector(0, 0, 1));
    const auto s = Sphere();

    const auto xs = s.intersect(r);

    REQUIRE(xs.size() == 2);
    REQUIRE(equal(xs.at(0).t(), 5.0));
    REQUIRE(equal(xs.at(1).t(), 5.0));
}

TEST_CASE("testRayMissesSphere")
{
    const auto r = Ray(Point(0, 2, -5), Vector(0, 0, 1));
    const auto s = Sphere();

    const auto xs = s.intersect(r);

    REQUIRE(xs.empty());
}

TEST_CASE("testRayOriginatesInsideSphere")
{
    const auto r = Ray(Point(0, 0, 0), Vector(0, 0, 1));
    const auto s = Sphere();

    const auto xs = s.intersect(r);

    REQUIRE(xs.size() == 2);
    REQUIRE(equal(xs.at(0).t(), -1.0));
    REQUIRE(equal(xs.at(1).t(), 1.0));
}

TEST_CASE("testRayIsInFrontOfSphere")
{
    const auto r = Ray(Point(0, 0, 5), Vector(0, 0, 1));
    const auto s = Sphere();

    const auto xs = s.intersect(r);

    REQUIRE(xs.size() == 2);
    REQUIRE(equal(xs.at(0).t(), -6.0));
    REQUIRE(equal(xs.at(1).t(), -4.0));
}

TEST_CASE("Spheres should have unique ids") {
    const auto s1 = Sphere();
    const auto s2 = Sphere();

    REQUIRE(s1.id() != s2.id());
}

TEST_CASE("An intersection encapsulates t and object") {
        const auto s = Sphere();
        const auto i = Intersection(3.5, s);
        REQUIRE(equal(i.t(), 3.5));
        REQUIRE(s == i.object());
}

TEST_CASE("Aggregating intersections") {
    const auto s = Sphere();
    const auto i1 = Intersection(1, s);
    const auto i2 = Intersection(2, s);

    const auto xs = intersections(i1, i2);

    REQUIRE(xs.size() == 2);
    REQUIRE(equal(xs.at(0).t(), 1));
    REQUIRE(equal(xs.at(1).t(), 2));
}

TEST_CASE("Intersect sets the object on the intersection") {
    const auto r = Ray(Point(0, 0, -5), Vector(0, 0, 1));
    const auto s = Sphere();
    const auto xs = s.intersect(r);

    REQUIRE(xs.size() == 2);

    REQUIRE(xs.at(0).object() == s);
    REQUIRE(xs.at(1).object() == s);
}

