#ifndef TUPLE_H
#define TUPLE_H

#include <iostream>

#define EPSILON 0.00001
bool equal(const double& a, const double& b);

class Tuple
{
public:
    Tuple(double x, double y, double z, double w);

public:

    Tuple operator -() const;
    Tuple operator +(const Tuple& tuple) const;
    Tuple operator -(const Tuple& tuple) const;
    Tuple operator *(const double& multiplier) const;
    Tuple operator /(const double& divider) const;
    bool operator ==(const Tuple& tuple) const;

    bool isPoint() const;
    bool isVector() const;

    inline double x() const { return m_x; }
    inline double y() const { return m_y; }
    inline double z() const { return m_z; }
    inline double w() const { return m_w; }

    inline void resetWToZero() {
        m_w = 0;
    }

    friend std::ostream &operator<<(std::ostream &os, const Tuple& t) {
        return os << (t.isVector() ? "Vector" : "Point") << "(" << t.x() << ", " << t.y() << ", " << t.z() << ")";
    }

protected:
    double m_x;
    double m_y;
    double m_z;
private:
    double m_w;
};

#endif // TUPLE_H
