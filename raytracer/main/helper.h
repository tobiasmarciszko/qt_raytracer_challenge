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

public slots:
    void tick();

private:
    IEffect *effect;
};

#endif
