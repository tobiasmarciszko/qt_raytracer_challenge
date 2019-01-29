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
