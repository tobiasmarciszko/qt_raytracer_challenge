#ifndef HELPER_H
#define HELPER_H

#include <QBrush>
#include <QFont>
#include <QPen>
#include <QWidget>
#include <QImage>
#include "effect.h"

class Helper : public QObject
{
    Q_OBJECT

public:
    explicit Helper(QObject *parent = nullptr);
    ~Helper() final;

public:
    void paint(QPainter *painter);
    void fromX(double arg) { effect->fromX = arg; }
    void fromY(double arg) { effect->fromY = arg; }
    void fromZ(double arg) { effect->fromZ = arg; }
    void toX(double arg) { effect->toX = arg; }
    void toY(double arg) { effect->toY = arg; }
    void toZ(double arg) { effect->toZ = arg; }

public slots:
    void tick();

public:
    IEffect *effect;
};

#endif
