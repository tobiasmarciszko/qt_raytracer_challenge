#ifndef PATTERN_H
#define PATTERN_H

#include "color.h"
#include "point.h"
#include <cmath>

// Predefined colors for convenience
const Color black = Color(0,0,0);
const Color white = Color(1,1,1);

struct Pattern {

    Pattern() = default;
    Pattern(const Color& _a, const Color& _b) : a(_a), b(_b) {}

    Color a;
    Color b;

    Color stripe_at(const Point& p) const {

        if (static_cast<int>(std::floor(p.x)) % 2 == 0) {
            return a;
        }

        return b;
    }
};

inline Pattern stripe_pattern(const Color& a, const Color &b) {
    return Pattern(a, b);
}


#endif // PATTERN_H
