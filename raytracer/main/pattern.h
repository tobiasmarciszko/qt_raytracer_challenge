#ifndef PATTERN_H
#define PATTERN_H

#include "color.h"

const Color black = Color(0,0,0);
const Color white = Color(1,1,1);

struct Pattern {
    Color a;
    Color b;
};

inline Pattern stripe_pattern(const Color& a, const Color &b) {
    return Pattern();
}

#endif // PATTERN_H
