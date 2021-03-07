#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "imageitem.h"
#include "raytracerbackend.h"
#include "shapeqmlbridge.h"
#include "appsettings.h"

int main(int argc, char *argv[])
{
    // App settings
    // qmlRegisterType<AppSettings>("myextension", 1, 0, "AppSettings");

    // Make sure the raytracer backend outlives the QML engine to avoid
    // binding errors on exit. Objects are destructed in reverse order of
    // construction.
    RaytracerBackend raytracer;

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QCoreApplication::setOrganizationName("Tobias Marciszko");
    QCoreApplication::setApplicationName("Freeside Raytracer");

    QQmlApplicationEngine engine;

    const QUrl url("qrc:/raytracer.qml");
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.rootContext()->setContextProperty("raytracer", &raytracer);
    engine.rootContext()->setContextProperty("settings", &AppSettings::get());

    // This is the item that draw the QImage from the Raytracer
    qmlRegisterType<ImageItem>("myextension", 1, 0, "ImageItem");

    // Experimenting with exposing pure C++ classes through to QML
    // by using bridge/container classes. Is this a good idea?
    qmlRegisterType<ShapeQmlBridge>("myextension", 1, 0, "ShapeQmlBridge");

    engine.load(url);

    return QGuiApplication::exec();
}
