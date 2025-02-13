#include "../include/connection_manager.h"

#include <QHostAddress>
#include <QDebug>
#include <QTimer>

BroadcastConnectionManager::BroadcastConnectionManager(QObject *parent)
    : QObject(parent)
    , m_udpSocket(new QUdpSocket(this))
    , m_tcpSocket(new QTcpSocket(this))
    , m_isConnected(false)
{
    // udp soket baglanti
    connect(m_udpSocket, &QUdpSocket::readyRead, this, &BroadcastConnectionManager::onUdpReadyRead);

    // tcp soket baglanti
    connect(m_tcpSocket, &QTcpSocket::connected,
            this, &BroadcastConnectionManager::onTcpConnected);
    connect(m_tcpSocket, &QTcpSocket::disconnected,
            this, &BroadcastConnectionManager::onTcpDisconnected);
    connect(m_tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
            this, &BroadcastConnectionManager::onTcpErrorOccurred);
}

bool BroadcastConnectionManager::isConnected() const
{
    return m_isConnected;
}

void BroadcastConnectionManager::startListeningUdp(quint16 port)
{
    if (m_isConnected) {
        qDebug() << "TCP zaten bagli ve UDP Broadcast' e ihtiyac yok.";
        return;
    }

    if (m_udpSocket->isOpen()) {
        m_udpSocket->close();
    }

    bool success = m_udpSocket->bind(QHostAddress::AnyIPv4, port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    if (!success) {
        qWarning() << "UDP soket hatasi.";
    } else {
        qDebug() << "UDP dinleme: " << port;
    }
}

void BroadcastConnectionManager::stopListeningUdp()
{
    if (m_udpSocket->isOpen()) {
        m_udpSocket->close();
        qDebug() << "UDP dinleme durduruldu.";
    }
}

void BroadcastConnectionManager::disconnectTcp()
{
    if (m_tcpSocket->isOpen()) {
        m_tcpSocket->disconnectFromHost();
    }
}

void BroadcastConnectionManager::onUdpReadyRead()
{
    while (m_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(static_cast<int>(m_udpSocket->pendingDatagramSize()));
        QHostAddress sender;
        quint16 senderPort;

        m_udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        QString message = QString::fromUtf8(datagram);

        emit udpMessageReceived(message);

        QString vehicleName;
        QString ip;
        quint16 tcpPort = 0;

        if (parseBroadcast(message, vehicleName, ip, tcpPort)) {
            connectTcp(ip, tcpPort);
        } else {
            qWarning() << "Gecersiz broadcast mesaji:" << message;
        }
    }
}

void BroadcastConnectionManager::onTcpConnected()
{
    if (!m_isConnected) {
        m_isConnected = true;
        emit isConnectedChanged();
        qDebug() << "TCP baglandi, UDP dinleme durduruldu.";
        stopListeningUdp();  // tcp baglaninca udp dinleme durduulur.
    }

    emit connectedToServer();
}

void BroadcastConnectionManager::onTcpDisconnected()
{
    if (m_isConnected) {
        m_isConnected = false;
        emit isConnectedChanged();
    }

    emit disconnectedFromServer();
}

void BroadcastConnectionManager::onTcpErrorOccurred(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);

    if (m_isConnected) {
        m_isConnected = false;
        emit isConnectedChanged();
    }

    emit connectionError(m_tcpSocket->errorString());
}

bool BroadcastConnectionManager::parseBroadcast(const QString &broadcast,
                                                QString &outVehicleName,
                                                QString &outIp,
                                                quint16 &outPort)
{
    QStringList lines = broadcast.split('\n', Qt::SkipEmptyParts);
    if (lines.size() < 3)
        return false;


    if (!lines.at(0).startsWith("hermod-polaris-broadcast")) { //broadcastin aractan geldigini gosteren tanimlayici
        return false;
    }

    qDebug() << "Aractan gelmis!";

    outVehicleName = lines.at(1).trimmed();
    QString addressInfo = lines.at(2).trimmed();
    if (!addressInfo.contains(":")) {
        return false;
    }
    QStringList ipPortParts = addressInfo.split(":");
    if (ipPortParts.size() != 2)
        return false;

    outIp = ipPortParts.at(0).trimmed();
    bool ok = false;
    quint16 parsedPort = ipPortParts.at(1).toUShort(&ok);
    if (!ok)
        return false;

    outPort = parsedPort;
    return true;
}

void BroadcastConnectionManager::connectTcp(const QString &ip, quint16 port)
{
    if (m_isConnected)
    {
        return;
    }

    if (m_tcpSocket->isOpen()) {
        m_tcpSocket->disconnectFromHost();
    }

    qDebug() << "TCP baglanmaya calisiyor: " << ip << ":" << port;
    
    m_tcpSocket->connectToHost(ip, port);
}
