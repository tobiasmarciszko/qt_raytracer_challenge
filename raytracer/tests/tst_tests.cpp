#include <QString>
#include <QtTest>
#include "tuple.h"

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
    Tuple point = Tuple::Point(1, 2, 3);
    QVERIFY2(point.isPoint(), "Not a point!");
}

void Tests::testVector()
{
    Tuple vector = Tuple::Vector(1, 2, 3);
    QVERIFY2(vector.isVector(), "Not a vector!");
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
    Tuple p1 = Tuple::Point(3, 2, 1);
    Tuple p2 = Tuple::Point(5, 6, 7);

    Tuple v1 = p1 - p2;

    QVERIFY(v1.isVector());
    QVERIFY(equal(-2, v1.x()));
    QVERIFY(equal(-4, v1.y()));
    QVERIFY(equal(-6, v1.z()));
}

void Tests::testSubtractVectorFromPoint()
{
    Tuple p = Tuple::Point(3, 2, 1);
    Tuple v = Tuple::Vector(5, 6, 7);

    Tuple p1 = p - v;

    QVERIFY(p1.isPoint());
    QVERIFY(equal(-2, p1.x()));
    QVERIFY(equal(-4, p1.y()));
    QVERIFY(equal(-6, p1.z()));
}

void Tests::testSubtractVectors()
{
    Tuple v1 = Tuple::Vector(3, 2, 1);
    Tuple v2 = Tuple::Vector(5, 6, 7);

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

QTEST_APPLESS_MAIN(Tests)

#include "tst_tests.moc"
