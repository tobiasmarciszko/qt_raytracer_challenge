#include "glwidget.h"

#include <QPainter>
#include <QTimer>
#include <QOpenGLWidget>
#include "helper.h"
#include "mainwindow.h"

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    helper = ((MainWindow*)(parent)->parent())->getHelper();
    setFixedSize(320, 240);
    setAutoFillBackground(false);
}

void GLWidget::animate()
{
    update();
}

void GLWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    helper->paint(&painter);
    painter.end();
}
