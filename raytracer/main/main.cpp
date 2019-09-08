#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "imageitem.h"
#include "raytracer.h"
#include "shapeqmlbridge.h"

int main(int argc, char *argv[])
{
    // This is the item that draw the QImage from the Raytracer
    qmlRegisterType<ImageItem>("myextension", 1, 0, "ImageItem");

    // Experimenting with exposing pure C++ classes through to QML
    // by using bridge/container classes. Is this a good idea?
    qmlRegisterType<ShapeQmlBridge>("myextension", 1, 0, "ShapeBridge");

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/raytracer.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    Raytracer raytracer;
    engine.rootContext()->setContextProperty("raytracer", &raytracer);
    engine.load(url);

    return app.exec();
}
