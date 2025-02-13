#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "include/connection_manager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    BroadcastConnectionManager connectionManager;
    
    QQmlApplicationEngine engine;
    // context property leri modu eklemeeden once cagir
    engine.rootContext()->setContextProperty("connectionManager", &connectionManager);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("desktop", "Main");

    

    return app.exec();
}
