#include <QCoreApplication>
#include "tuple.h"

int main(int argc, char *argv[])
{
    Tuple tuple(1.0, 2.0, 3.0, 4.0);

    QCoreApplication a(argc, argv);
    return a.exec();
}
