#include <QString>
#include <QtTest>
#include <QDebug>
#include <cmath>
#include "tuple.h"
#include "point.h"
#include "vector.h"
#include "color.h"
#include "canvas.h"
#include "matrix.h"

const double pi = std::acos(-1);

class Tests : public QObject
{
    Q_OBJECT

public:
    Tests();

private Q_SLOTS:
    void testTuple();
    void testPoint();
    void testVector();
    void testAdd();
    void testSubtractPoints();
    void testSubtractVectorFromPoint();
    void testSubtractVectors();
    void testNegate();
    void testScalarMultiply();
    void testFractionMultiply();
    void testScalarDivision();
    void testMagnitude();
    void testNormalize();
    void testDot();
    void testCross();

    // Chapter 2
    void testColor();
    void testColorAdd();
    void testColorSubtract();
    void testColorScalar();
    void testColorHadamard();
    void testCanvas();

    // Chapter 3
    void testMatrix4x4();
    void testMatrix3x3();
    void testMatrix2x2();

    void testMatrixEqual();
    void testMatrixNotEqual();
    void testMatrixMultiplication();
    void testMatrixTupleMultiplication();
    void testMultiplicationWithIdentityMatrix();
    void testMatrixTranspose();
    void testMatrixTransposeIdentity();
    void testMatrix2x2Determinant();
    void test2x2SubMatrix();
    void test3x3SubMatrix();
    void testMinor();
    void testCofactor();
    void testMatrix3x3Determinant();
    void testMatrix4x4Determinant();
    void testInvertible();
    void testNonInvertible();
    void testInverse();
    void testInverseAnother();
    void testInverseThird();
    void testInverseMultiply();

    // Chapter 4
    void testTranslation();
    void testInverseTranslation();
    void testTranslateVector();
    void testScalePoint();
    void testScaleVector();
    void testInverseScale();
    void testReflection();

    void testRotateX();
    void testRotateInverseX();

    void testRotateY();
    void testRotateInverseY();

    void testRotateZ();
    void testRotateInverseZ();
};

Tests::Tests() = default;

void Tests::testTuple()
{
    Tuple tuple(1.0, 2.0, 3.0, 4.0);
    QVERIFY2(equal(1.0, tuple.x()), "x is not set correctly");
    QVERIFY2(equal(2.0, tuple.y()), "y is not set correctly");
    QVERIFY2(equal(3.0, tuple.z()), "z is not set correctly");
    QVERIFY2(equal(4.0, tuple.w()), "w is not set correctly");
}

void Tests::testPoint()
{
    Point point = Point(1, 2, 3);
    QVERIFY2(point.isPoint(), "Not a point!");

    Tuple tuple = point;
    QVERIFY(equal(1, tuple.w()));
}

void Tests::testVector()
{
    Vector vector = Vector(1, 2, 3);
    QVERIFY2(vector.isVector(), "Not a vector!");

    Tuple tuple = vector;
    QVERIFY(equal(0, tuple.w()));
}

void Tests::testAdd()
{
    Tuple a1 = Tuple(3, -2, 5, 1);
    Tuple a2 = Tuple(-2, 3, 1, 0);

    Tuple a3 = a1 + a2;

    QVERIFY(equal(1, a3.x()));
    QVERIFY(equal(1, a3.y()));
    QVERIFY(equal(6, a3.z()));
    QVERIFY(equal(1, a3.w()));
}

void Tests::testSubtractPoints()
{
    Point p1 = Point(3, 2, 1);
    Point p2 = Point(5, 6, 7);

    Vector v1 = p1 - p2;

    QVERIFY(v1.isVector());
    QVERIFY(equal(-2, v1.x()));
    QVERIFY(equal(-4, v1.y()));
    QVERIFY(equal(-6, v1.z()));
}

void Tests::testSubtractVectorFromPoint()
{
    Point p = Point(3, 2, 1);
    Vector v = Vector(5, 6, 7);

    Point p1 = p - v;

    QVERIFY(p1.isPoint());
    QVERIFY(equal(-2, p1.x()));
    QVERIFY(equal(-4, p1.y()));
    QVERIFY(equal(-6, p1.z()));
}

void Tests::testSubtractVectors()
{
    Vector v1 = Vector(3, 2, 1);
    Vector v2 = Vector(5, 6, 7);

    Vector v3 = v1 - v2;

    QVERIFY(v3.isVector());
    QVERIFY(equal(-2, v3.x()));
    QVERIFY(equal(-4, v3.y()));
    QVERIFY(equal(-6, v3.z()));
}

void Tests::testNegate()
{
    Tuple tuple = -Tuple(1, -2, 3, -4);

    QVERIFY(equal(-1, tuple.x()));
    QVERIFY(equal(2, tuple.y()));
    QVERIFY(equal(-3, tuple.z()));
    QVERIFY(equal(4, tuple.w()));
}

void Tests::testScalarMultiply()
{
    Tuple a = Tuple(1, -2, 3, -4);
    Tuple tuple = a * 3.5;

    QVERIFY(equal(3.5, tuple.x()));
    QVERIFY(equal(-7, tuple.y()));
    QVERIFY(equal(10.5, tuple.z()));
    QVERIFY(equal(-14, tuple.w()));
}

void Tests::testFractionMultiply()
{
    Tuple a = Tuple(1, -2, 3, -4);
    Tuple tuple = a * 0.5;

    QVERIFY(equal(0.5, tuple.x()));
    QVERIFY(equal(-1, tuple.y()));
    QVERIFY(equal(1.5, tuple.z()));
    QVERIFY(equal(-2, tuple.w()));
}

void Tests::testScalarDivision()
{
    Tuple a = Tuple(1, -2, 3, -4);
    Tuple tuple = a / 2;

    QVERIFY(equal(0.5, tuple.x()));
    QVERIFY(equal(-1, tuple.y()));
    QVERIFY(equal(1.5, tuple.z()));
    QVERIFY(equal(-2, tuple.w()));
}

void Tests::testMagnitude()
{
    Vector v1 = Vector(1,0,0);
    QVERIFY(equal(1, v1.magnitude()));

    Vector v2 = Vector(0,1,0);
    QVERIFY(equal(1, v2.magnitude()));

    Vector v3 = Vector(0,0,1);
    QVERIFY(equal(1, v3.magnitude()));

    Vector v4 = Vector(1,2,3);
    QVERIFY(equal(sqrt(14), v4.magnitude()));

    Vector v5 = Vector(-1,-2,-3);
    QVERIFY(equal(sqrt(14), v5.magnitude()));
}

void Tests::testNormalize()
{
    Vector v1 = Vector(4,0,0);
    Vector v2 = v1.normalize();

    QVERIFY(equal(1, v2.x()));

    Vector v3 = Vector(1,2,3);
    Vector v4 = v3.normalize();

    QVERIFY(equal(1 / sqrt(14), v4.x()));
    QVERIFY(equal(2 / sqrt(14), v4.y()));
    QVERIFY(equal(3 / sqrt(14), v4.z()));
}

void Tests::testDot()
{
    Vector a = Vector(1,2,3);
    Vector b = Vector(2,3,4);

    double dot = a.dot(b);

    QVERIFY(equal(20, dot));
}

void Tests::testCross()
{
    Vector a = Vector(1,2,3);
    Vector b = Vector(2,3,4);

    Vector cross1 = a.cross(b);
    QVERIFY(equal(-1, cross1.x()));
    QVERIFY(equal(2, cross1.y()));
    QVERIFY(equal(-1, cross1.z()));

    Vector cross2 = b.cross(a);
    QVERIFY(equal(1, cross2.x()));
    QVERIFY(equal(-2, cross2.y()));
    QVERIFY(equal(1, cross2.z()));

}

// Chapter 2

void Tests::testColor()
{
    Color c = Color(-0.5, 0.4, 1.7);

    QVERIFY(equal(-0.5, c.red()));
    QVERIFY(equal(0.4, c.green()));
    QVERIFY(equal(1.7, c.blue()));
}

void Tests::testColorAdd()
{
    Color c1 = Color(0.9, 0.6, 0.75);
    Color c2 = Color(0.7, 0.1, 0.25);

    Color c3 = c1 + c2;

    QVERIFY(equal(1.6, c3.red()));
    QVERIFY(equal(0.7, c3.green()));
    QVERIFY(equal(1.0, c3.blue()));
}

void Tests::testColorSubtract()
{
    Color c1 = Color(0.9, 0.6, 0.75);
    Color c2 = Color(0.7, 0.1, 0.25);

    Color c3 = c1 - c2;

    QVERIFY(equal(0.2, c3.red()));
    QVERIFY(equal(0.5, c3.green()));
    QVERIFY(equal(0.5, c3.blue()));
}

void Tests::testColorScalar()
{
    Color c1 = Color(0.2, 0.3, 0.4);
    Color c2 = c1 * 2;

    QVERIFY(equal(0.4, c2.red()));
    QVERIFY(equal(0.6, c2.green()));
    QVERIFY(equal(0.8, c2.blue()));
}

void Tests::testColorHadamard() {

    Color c1 = Color(1, 0.2, 0.4);
    Color c2 = Color(0.9, 1, 0.1);

    Color c3 = c1 * c2;

    QVERIFY(equal(0.9, c3.red()));
    QVERIFY(equal(0.2, c3.green()));
    QVERIFY(equal(0.04, c3.blue()));
}

void Tests::testCanvas() {
    Canvas<10,20> c;
    Color red = Color(1, 0, 0);

    c.write_pixel(0, 0, red);
    QVERIFY(equal(1, c.pixel_at(0, 0).red()));

    c.write_pixel(10 - 1, 20 - 1, red);
    QVERIFY(equal(1, c.pixel_at(10 - 1, 20 - 1).red()));
}

void Tests::testMatrix4x4() {
    const auto m = Matrix<4,4>(
                                1,    2,    3,    4,
                                5.5,  6.5,  7.5,  8.5,
                                9,    10,   11,   12,
                                13.5, 14.5, 15.5, 16.5);

    QVERIFY(equal(1,    m.get(0, 0)));
    QVERIFY(equal(4,    m.get(0, 3)));
    QVERIFY(equal(5.5,  m.get(1, 0)));
    QVERIFY(equal(7.5,  m.get(1, 2)));
    QVERIFY(equal(11,   m.get(2, 2)));
    QVERIFY(equal(13.5, m.get(3, 0)));
    QVERIFY(equal(15.5, m.get(3, 2)));
}

void Tests::testMatrix3x3() {
    const auto m = Matrix<3,3>(-3,    5,   0,
                                1,   -2,  -7,
                                0,    1,   1);

    QVERIFY(equal(-3, m.get(0, 0)));
    QVERIFY(equal(-2, m.get(1, 1)));
    QVERIFY(equal(1,  m.get(2, 2)));
}

void Tests::testMatrix2x2() {
    const auto m = Matrix<2,2>(-3,  5,
                                1, -2);

    QVERIFY(equal(-3, m.get(0, 0)));
    QVERIFY(equal(5,  m.get(0, 1)));
    QVERIFY(equal(1,  m.get(1, 0)));
    QVERIFY(equal(-2,  m.get(1, 1)));
}

void Tests::testMatrixEqual() {
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

    QVERIFY(a == b);

}

void Tests::testMatrixNotEqual() {
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

    QVERIFY(a != b);
}

void Tests::testMatrixMultiplication() {
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

    QVERIFY(result == expected);

}

void Tests::testMatrixTupleMultiplication() {
    const auto a = Matrix<4,4>(
                1,2,3,4,
                2,4,4,2,
                8,6,4,1,
                0,0,0,1);

    const auto b = Tuple(1, 2, 3, 1);

    const auto expected = Tuple(18, 24, 33, 1);

    const auto result = a * b;

    QVERIFY(result == expected);
}

void Tests::testMultiplicationWithIdentityMatrix() {

    const auto A = Matrix<4,4>(
                0,1,2,4,
                1,2,4,8,
                2,4,8,16,
                4,8,16,32);

    QVERIFY((A *identity_matrix) == A);

    const auto a = Tuple(1, 2, 3, 4);

    QVERIFY((identity_matrix * a) == a);
}

void Tests::testMatrixTranspose() {

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

    QVERIFY(result == expected);
}

void Tests::testMatrixTransposeIdentity()
{
    // Transposing the identity matrix should be the... identity matrix! :)

    const auto result = identity_matrix.transpose();
    QVERIFY(identity_matrix == result);
}

void Tests::testMatrix2x2Determinant()
{
    const auto A = Matrix<2,2>(
                1, 5,
                -3, 2);

    const auto expected = 17;
    const auto result = A.determinant();

    QVERIFY(equal(result, expected));
}

void Tests::test2x2SubMatrix()
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

    QVERIFY(expected == result);
}

void Tests::test3x3SubMatrix()
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

    QVERIFY(expected == result);
}

void Tests::testMinor()
{
    const auto A = Matrix<3,3>(
                3,5,0,
                2,-1,-7,
                6,-1,5);

    const auto B = A.submatrix(1,0);

    const auto expected = 25;

    const auto determinant = B.determinant();
    const auto minor = A.minor(1,0);

    QVERIFY(equal(expected, determinant));
    QVERIFY(equal(expected, minor));
}

void Tests::testCofactor()
{
    const auto A = Matrix<3,3>(
                3,5,0,
                2,-1,-7,
                6,-1,5);

    QVERIFY(equal(A.minor(0,0), -12));
    QVERIFY(equal(A.cofactor(0,0), -12));
    QVERIFY(equal(A.minor(1,0), 25));
    QVERIFY(equal(A.cofactor(1,0), -25));
}

void Tests::testMatrix3x3Determinant()
{
    const auto A = Matrix<3,3>(
                1,2,6,
                -5,8,-4,
                2,6,4);

    QVERIFY(equal(A.cofactor(0,0), 56));
    QVERIFY(equal(A.cofactor(0,1), 12));
    QVERIFY(equal(A.cofactor(0,2), -46));
    QVERIFY(equal(A.determinant(), -196));
}

void Tests::testMatrix4x4Determinant()
{
    const auto A = Matrix<4,4>(
                -2,-8,3,5,
                -3,1,7,3,
                1,2,-9,6,
                -6,7,7,-9);

    QVERIFY(equal(A.cofactor(0,0), 690));
    QVERIFY(equal(A.cofactor(0,1), 447));
    QVERIFY(equal(A.cofactor(0,2), 210));
    QVERIFY(equal(A.cofactor(0,3), 51));
    QVERIFY(equal(A.determinant(), -4071));
}

void Tests::testInvertible()
{
    const auto A = Matrix<4,4>(
                6,4,4,4,
                5,5,7,6,
                4,-9,3,-7,
                9,1,7,-6);

    QVERIFY(equal(A.determinant(), -2120));
    QVERIFY(A.invertible());
}

void Tests::testNonInvertible()
{
    const auto A = Matrix<4,4>(
                -4,2,-2,-3,
                9,6,2,6,
                0,-5,1,-5,
                0,0,0,0);

    QVERIFY(equal(A.determinant(), 0));
    QVERIFY(A.invertible() == false);
}

void Tests::testInverse()
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

    QVERIFY(equal(A.determinant(), 532.0));
    QVERIFY(equal(A.cofactor(2,3), -160.0));
    QVERIFY(equal(B.get(3,2), -160.0/532.0));
    QVERIFY(equal(A.cofactor(3,2), 105.0));
    QVERIFY(equal(B.get(2,3), 105.0/532.0));

    QVERIFY(B == expected);
}

void Tests::testInverseAnother()
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

    QVERIFY(A.inverse() == expected);
}

void Tests::testInverseThird()
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

    QVERIFY(A.inverse() == expected);
}

void Tests::testInverseMultiply()
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

    QVERIFY((C * B.inverse()) == A);
}

void Tests::testTranslation()
{
    const auto transform = translation(5, -3, 2);
    const auto p = Point(-3, 4, 5);

    const auto result = transform * p;

    QVERIFY(result == Point(2, 1, 7));
}

void Tests::testInverseTranslation()
{
    const auto transform = translation(5, -3, 2);
    const auto inv = transform.inverse();
    const auto p = Point(-3, 4, 5);

    const auto result = inv * p;

    QVERIFY(result == Point(-8, 7, 3));
}

void Tests::testTranslateVector()
{
    const auto transform = translation(5, -3, 2);
    const auto v = Vector(-3, 4, 5);

    const auto result = transform * v;

    QVERIFY(result == v);
}

void Tests::testScalePoint()
{
    const auto transform = scaling(2, 3, 4);
    const auto p = Point(-4, 6, 8);

    const auto result = transform * p;

    QVERIFY(result == Point(-8, 18, 32));
}

void Tests::testScaleVector()
{
    const auto transform = scaling(2, 3, 4);
    const auto v = Vector(-4, 6, 8);

    const auto result = transform * v;

    QVERIFY(result == Vector(-8, 18, 32));
}

void Tests::testInverseScale()
{
    const auto transform = scaling(2, 3, 4);
    const auto inv = transform.inverse();
    const auto v = Vector(-4, 6, 8);

    const auto result = inv * v;

    QVERIFY(result == Vector(-2, 2, 2));
}

void Tests::testReflection()
{
    const auto transform = scaling(-1, 1, 1);
    const auto p = Point(2, 3, 4);

    const auto result = transform * p;

    QVERIFY(result == Point(-2, 3, 4));
}

void Tests::testRotateX()
{
    const auto p = Point(0, 1, 0);
    const auto half_quarter = rotation_x(pi / 4);
    const auto full_quarter = rotation_x(pi / 2);

    QVERIFY(half_quarter * p == Point(0, std::sqrt(2) / 2, std::sqrt(2) / 2));
    QVERIFY(full_quarter * p == Point(0, 0, 1));
}

void Tests::testRotateInverseX()
{
    const auto p = Point(0, 1, 0);
    const auto half_quarter = rotation_x(pi / 4);
    const auto inv = half_quarter.inverse();

    QVERIFY(inv * p == Point(0, std::sqrt(2) / 2, - std::sqrt(2) / 2));
}

void Tests::testRotateY()
{
    const auto p = Point(0, 0, 1);
    const auto half_quarter = rotation_y(pi / 4);
    const auto full_quarter = rotation_y(pi / 2);

    QVERIFY(half_quarter * p == Point(std::sqrt(2) / 2, 0, std::sqrt(2) / 2));
    QVERIFY(full_quarter * p == Point(1, 0, 0));
}

void Tests::testRotateInverseY()
{
    const auto p = Point(0, 0, 1);
    const auto half_quarter = rotation_y(pi / 4);
    const auto inv = half_quarter.inverse();

    QVERIFY(inv * p == Point(-std::sqrt(2) / 2, 0, std::sqrt(2) / 2));
}


void Tests::testRotateZ()
{
    const auto p = Point(0, 1, 0);
    const auto half_quarter = rotation_z(pi / 4);
    const auto full_quarter = rotation_z(pi / 2);

    QVERIFY(half_quarter * p == Point(-std::sqrt(2) / 2, std::sqrt(2) / 2, 0));
    QVERIFY(full_quarter * p == Point(-1, 0, 0));
}

void Tests::testRotateInverseZ()
{
    const auto p = Point(0, 1, 0);
    const auto half_quarter = rotation_z(pi / 4);
    const auto inv = half_quarter.inverse();

    QVERIFY(inv * p == Point(std::sqrt(2) / 2, std::sqrt(2) / 2, 0));
}

QTEST_APPLESS_MAIN(Tests)

#include "tst_tests.moc"
