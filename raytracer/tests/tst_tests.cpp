#include <QString>
#include <QtTest>
#include "tuple.h"
#include "point.h"
#include "vector.h"

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

    Tuple v1 = p1 - p2;

    QVERIFY(v1.isVector());
    QVERIFY(equal(-2, v1.x()));
    QVERIFY(equal(-4, v1.y()));
    QVERIFY(equal(-6, v1.z()));
}

void Tests::testSubtractVectorFromPoint()
{
    Point p = Point(3, 2, 1);
    Vector v = Vector(5, 6, 7);

    Tuple p1 = p - v;

    QVERIFY(p1.isPoint());
    QVERIFY(equal(-2, p1.x()));
    QVERIFY(equal(-4, p1.y()));
    QVERIFY(equal(-6, p1.z()));
}

void Tests::testSubtractVectors()
{
    Vector v1 = Vector(3, 2, 1);
    Vector v2 = Vector(5, 6, 7);

    Tuple v3 = v1 - v2;

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

QTEST_APPLESS_MAIN(Tests)

#include "tst_tests.moc"
