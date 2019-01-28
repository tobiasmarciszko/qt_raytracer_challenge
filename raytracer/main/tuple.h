#ifndef TUPLE_H
#define TUPLE_H

#include <QObject>

#define EPSILON = 0.00001;

class Tuple : public QObject
{
    Q_OBJECT
public:
    explicit Tuple(QObject *parent = nullptr);
    Tuple(qreal x, qreal y, qreal z, qreal w);

signals:

public slots:
    static Tuple* Point(qreal x, qreal y, qreal z);
    static Tuple* Vector(qreal x, qreal y, qreal z);

    static bool Equal(qreal a, qreal b);

    bool isPoint();
    bool isVector();

    inline qreal x() { return m_x; }
    inline qreal y() { return m_y; }
    inline qreal z() { return m_z; }
    inline qreal w() { return m_w; }

private:
    qreal m_x;
    qreal m_y;
    qreal m_z;
    qreal m_w;
};

#endif // TUPLE_H
