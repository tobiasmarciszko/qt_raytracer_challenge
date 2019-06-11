#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "imageitem.h"
#include "raytracer.h"

int main(int argc, char *argv[])
{
    qmlRegisterType<ImageItem>("myextension", 1, 0, "ImageItem");

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    Raytracer raytracer;

    const QUrl url(QStringLiteral("qrc:/raytracer.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.rootContext()->setContextProperty("raytracer", &raytracer);
    engine.load(url);

    raytracer.update();

    return app.exec();
}
