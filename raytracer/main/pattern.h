#ifndef PATTERN_H
#define PATTERN_H

#include "color.h"
#include "point.h"
#include "matrix.h"
#include <cmath>
#include <memory>
#include <QDebug>
#include <QtGui/qrgb.h>
#include <QtGui/qimage.h>

// Predefined colors for convenience
const Color black = Color(0,0,0);
const Color white = Color(1,1,1);

struct Pattern {
    virtual ~Pattern() = default;

    Matrix<4,4> transform = identity_matrix;
    Matrix<4,4> inverse_transform = transform.inverse();

    virtual Color pattern_at(const Point& p) const = 0;

    inline void set_transform(const Matrix<4,4>& t) {
        transform = t;
        inverse_transform = transform.inverse();
    }
};

///////////////// Cloud Pattern /////////////////////
struct CloudPattern : public Pattern {

    QImage *clouds = new QImage(":/clouds");

    Color pattern_at(const Point& p) const override {
        int y = (abs(int(p.z))) % clouds->height();
        int x = (abs(int(p.x))) % clouds->width();

        const QColor color = clouds->pixelColor(x, y);
        return Color(color.redF(), color.greenF(), color.blueF());
    }
};


inline std::shared_ptr<Pattern> cloud_pattern() {
    return std::make_shared<CloudPattern>(CloudPattern());
}


///////////////// Test Pattern /////////////////////
struct TestPattern : public Pattern {
    Color pattern_at(const Point& p) const override {
        return Color(p.x, p.y, p.z);
    }
};


inline std::shared_ptr<Pattern> test_pattern() {
    return std::make_shared<TestPattern>(TestPattern());
}

///////////////// Stripe Pattern /////////////////////

struct StripePattern : public Pattern {

    StripePattern(const Color& _a, const Color& _b) : a(_a), b(_b) {}

    Color a;
    Color b;

    Color pattern_at(const Point& p) const override {
        if (static_cast<int>(std::floor(p.x)) % 2 == 0) {
            return a;
        }
        return b;
    }
};

inline std::shared_ptr<Pattern> stripe_pattern(const Color& a, const Color &b) {
    return std::make_shared<StripePattern>(StripePattern(a, b));
}

///////////////// XOR Pattern /////////////////////

struct XORPattern : public Pattern {

    Color pattern_at(const Point& p) const override {
        int c = (int)p.x^(int)p.y^(int)p.z;
        c = abs(c) % 256;

        float cfr = (float)(255-c) / 255.0;
        float cfg = (float)(c % 128) / 255.0;
        float cfb = 1 - cfr;

        return Color(cfr, cfg, cfb);
    }
};

inline std::shared_ptr<Pattern> xor_pattern() {
    return std::make_shared<XORPattern>(XORPattern());
}

///////////////// DOOM fire Pattern /////////////////////

const int FIRE_WIDTH  = 320;
const int FIRE_HEIGHT = 240;
static uchar firePixels[FIRE_WIDTH*FIRE_HEIGHT];

static QVector<QRgb> palette = {
    qRgb(0x07,0x07,0x07),
    qRgb(0x1F,0x07,0x07),
    qRgb(0x2F,0x0F,0x07),
    qRgb(0x47,0x0F,0x07),
    qRgb(0x57,0x17,0x07),
    qRgb(0x67,0x1F,0x07),
    qRgb(0x77,0x1F,0x07),
    qRgb(0x8F,0x27,0x07),
    qRgb(0x9F,0x2F,0x07),
    qRgb(0xAF,0x3F,0x07),
    qRgb(0xBF,0x47,0x07),
    qRgb(0xC7,0x47,0x07),
    qRgb(0xDF,0x4F,0x07),
    qRgb(0xDF,0x57,0x07),
    qRgb(0xDF,0x57,0x07),
    qRgb(0xD7,0x5F,0x07),
    qRgb(0xD7,0x5F,0x07),
    qRgb(0xD7,0x67,0x0F),
    qRgb(0xCF,0x6F,0x0F),
    qRgb(0xCF,0x77,0x0F),
    qRgb(0xCF,0x7F,0x0F),
    qRgb(0xCF,0x87,0x17),
    qRgb(0xC7,0x87,0x17),
    qRgb(0xC7,0x8F,0x17),
    qRgb(0xC7,0x97,0x1F),
    qRgb(0xBF,0x9F,0x1F),
    qRgb(0xBF,0x9F,0x1F),
    qRgb(0xBF,0xA7,0x27),
    qRgb(0xBF,0xA7,0x27),
    qRgb(0xBF,0xAF,0x2F),
    qRgb(0xB7,0xAF,0x2F),
    qRgb(0xB7,0xB7,0x2F),
    qRgb(0xB7,0xB7,0x37),
    qRgb(0xCF,0xCF,0x6F),
    qRgb(0xDF,0xDF,0x9F),
    qRgb(0xEF,0xEF,0xC7),
    qRgb(0xFF,0xFF,0xFF)
};

static void spreadFire(int src) {
    auto pixel = firePixels[src];
    if (pixel == 0) {
        firePixels[src - FIRE_WIDTH] = 0;
    } else {
        int randIdx = qrand() % 3;
        int dst = src - randIdx + 1;
        firePixels[dst - FIRE_WIDTH] = pixel - (randIdx & 1);
    }
}

static void doFire() {
    for (int x = 0 ; x < FIRE_WIDTH; x++) {
        for (int y = 1; y < FIRE_HEIGHT; y++) {
            spreadFire(y * FIRE_WIDTH + x);
        }
    }
}

struct DoomFirePattern : public Pattern {

    DoomFirePattern() {
        for (uint i = 0; i < FIRE_WIDTH*FIRE_HEIGHT; i++) {
            firePixels[i] = 0;
        }

        for(uint i = 0; i < FIRE_WIDTH; i++) {
            firePixels[(FIRE_HEIGHT-1)*FIRE_WIDTH + i] = 36;
        }

        for (int i = 0; i < 100; i++) {
            doFire();
        }
    }

    Color pattern_at(const Point& p) const override {
        int y = (220-abs(int(p.y)));
        int x = (int(p.x)) % 320;

        if (y > 240) return black;
        if (y < 0) return black;

        int index = firePixels[y * FIRE_WIDTH + x];

        if (index > 36) return black;

        QRgb color = palette[index];

        float cfr = qRed(color)/255.0F;
        float cfg = qGreen(color)/255.0F;
        float cfb = qBlue(color)/255.0F;

        return Color(cfr, cfg, cfb);
    }
};

inline std::shared_ptr<Pattern> doomfire_pattern() {
    return std::make_shared<DoomFirePattern>(DoomFirePattern());
}

#endif // PATTERN_H
