#ifndef CONNECTION_MANAGER_H
#define CONNECTION_MANAGER_H

#include <QObject>
#include <QUdpSocket>
#include <QTcpSocket>

class BroadcastConnectionManager : public QObject
{
    Q_OBJECT

    // qml erisimi icin qproperty ui dosyalarindan erisim saglar
    Q_PROPERTY(bool isConnected READ isConnected NOTIFY isConnectedChanged)

public:
    explicit BroadcastConnectionManager(QObject *parent = nullptr);

    // baglantiyi takip eder
    bool isConnected() const;

public slots:
    void startListeningUdp(quint16 port);
    void stopListeningUdp();
    void disconnectTcp();

private slots:
    // UDP verri okunabilir olduğunda cagrilir
    void onUdpReadyRead();
    // sinyalleri
    void onTcpConnected();
    void onTcpDisconnected();
    void onTcpErrorOccurred(QAbstractSocket::SocketError socketError);

signals:
    // baglanti olaylari
    void connectedToServer();
    void disconnectedFromServer();
    void connectionError(const QString &errorMsg);
    // udp broadcast mesaji alininca cagirlir
    void udpMessageReceived(const QString &message);

    // isConnected degisince bu sinyal cagrilir
    void isConnectedChanged();

private:
    // broadast mesajinin aractan geldigini dogrular ve baglanti bilgilerini cikarir
    bool parseBroadcast(const QString &broadcast,
                        QString &outVehicleName,
                        QString &outIp,
                        quint16 &outPort);

    // parse edilen bilgiler ile tcp baglantisi kurar
    void connectTcp(const QString &ip, quint16 port);

    QUdpSocket *m_udpSocket;
    QTcpSocket *m_tcpSocket;
    bool        m_isConnected;
};

#endif
