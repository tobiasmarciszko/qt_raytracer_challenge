#include <QString>
#include <QtTest>
#include <QDebug>
#include "tuple.h"
#include "point.h"
#include "vector.h"
#include "color.h"
#include "canvas.h"
#include "matrix.h"

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

};

Tests::Tests()
{
}

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
    Canvas c = Canvas(10, 20);
    Color red = Color(1, 0, 0);

    c.write_pixel(0, 0, red);
    QVERIFY(equal(1, c.pixel_at(0, 0).red()));

    c.write_pixel(10 - 1, 20 - 1, red);
    QVERIFY(equal(1, c.pixel_at(10 - 1, 20 - 1).red()));
}

void Tests::testMatrix4x4() {
    Matrix<4,4> m = Matrix<4,4>(
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

QTEST_APPLESS_MAIN(Tests)

#include "tst_tests.moc"
