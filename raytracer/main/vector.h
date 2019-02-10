#ifndef VECTOR_H
#define VECTOR_H

#include "tuple.h"

class Vector : public Tuple
{
public:
    Vector(double x, double y, double z);
    double magnitude() const;
};

#endif // VECTOR_H
