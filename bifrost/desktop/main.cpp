#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "include/udp_listener.h"
#include "include/tcp_client.h"
#include <string>
#include <iostream>
#include "yonerge.h"

int main(int argc, char *argv[])
{
    std::string dosyayolu;
    std::cout << "Lutfen yonerge dosyasinin yolunu girin";
    std::getline (cin,dosyayolu);
    Yonerge yonerge (dosyayolu);
    yonerge.dosya_bilgisi();
    
    QGuiApplication app(argc, argv);

    TCPClient client;
    
    UDPListener udp_listener(12321);
    udp_listener.setTCPClient(&client);

    QQmlApplicationEngine engine;
    // context property leri modu eklemeeden once cagir
    engine.rootContext()->setContextProperty("UDPListener", &udp_listener);
    engine.rootContext()->setContextProperty("TCPClient", &client);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("desktop", "Main");

    

    return app.exec();
}
