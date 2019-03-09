#include <QDebug>
#include <QApplication>
#include "mainwindow.h"

#include "tuple.h"
#include "point.h"
#include "vector.h"



int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();

}

