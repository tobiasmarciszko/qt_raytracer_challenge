QT += core
QT += gui
QT += widgets

CONFIG += c++14

TARGET = main
# CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    tuple.cpp \
    point.cpp \
    vector.cpp \
    color.cpp \
    canvas.cpp \
    mainwindow.cpp \
    helper.cpp \
    glwidget.cpp \
    raytracer.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
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
