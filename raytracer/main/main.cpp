#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "imageitem.h"
#include "raytracerbackend.h"
#include "shapeqmlbridge.h"
#include "appsettings.h"

int main(int argc, char *argv[])
{
    // This is the item that draw the QImage from the Raytracer
    qmlRegisterType<ImageItem>("myextension", 1, 0, "ImageItem");

    // Experimenting with exposing pure C++ classes through to QML
    // by using bridge/container classes. Is this a good idea?
    qmlRegisterType<ShapeQmlBridge>("myextension", 1, 0, "ShapeBridge");

    // App settings
    qmlRegisterType<AppSettings>("myextension", 1, 0, "AppSettings");

    QGuiApplication app(argc, argv);

    QCoreApplication::setOrganizationName("Tobias Marciszko");
    QCoreApplication::setApplicationName("Freeside Raytracer");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/raytracer.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    RaytracerBackend raytracer;
    engine.rootContext()->setContextProperty("raytracer", &raytracer);

    AppSettings settings;
    engine.rootContext()->setContextProperty("settings", &settings);
    engine.load(url);

    return QGuiApplication::exec();
}
