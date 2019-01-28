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
};

Tests::Tests()
{
}

void Tests::testTuple()
{
    Tuple tuple(1.0, 2.0, 3.0, 4.0);
    QVERIFY2(Tuple::Equal(1.0, tuple.x()), "x is not set correctly");
    QVERIFY2(Tuple::Equal(2.0, tuple.y()), "y is not set correctly");
    QVERIFY2(Tuple::Equal(3.0, tuple.z()), "z is not set correctly");
    QVERIFY2(Tuple::Equal(4.0, tuple.w()), "w is not set correctly");
}

void Tests::testPoint()
{
    Tuple *point = Tuple::Point(1, 2, 3);
    QVERIFY2(point->isPoint(), "Not a point!");
}

void Tests::testVector()
{
    Tuple *vector = Tuple::Vector(1, 2, 3);
    QVERIFY2(vector->isVector(), "Not a vector!");
}

QTEST_APPLESS_MAIN(Tests)

#include "tst_tests.moc"
