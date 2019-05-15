QT += core
QT += gui
QT += widgets

CONFIG += c++17

TARGET = raytracer
TEMPLATE = app

SOURCES += main.cpp \
    intersection.cpp \
    object.cpp \
    ray.cpp \
    sphere.cpp \
    tuple.cpp \
    point.cpp \
    vector.cpp \
    color.cpp \
    mainwindow.cpp \
    helper.cpp \
    glwidget.cpp \
    raytracer.cpp

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    intersection.h \
    object.h \
    ray.h \
    sphere.h \
    tuple.h \
    point.h \
    vector.h \
    color.h \
    canvas.h \
    mainwindow.h \
    helper.h \
    glwidget.h \
    effect.h \
    raytracer.h \
    matrix.h

FORMS += \
    mainwindow.ui
