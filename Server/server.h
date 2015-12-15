#ifndef SERVER_H
#define SERVER_H
#include <QObject>
#include <QtCore>
QT_BEGIN_NAMESPACE
class QUdpSocket;
class QTcpServer;
class QTcpSocket;
class QNetworkSession;
QT_END_NAMESPACE

class server : public QObject
{
    Q_OBJECT

public:
    server();
    server(int port, bool isStartdByClient);
private:
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    QNetworkSession *networkSession;
    QUdpSocket *udpSocket;
    void broadcastDatagram();
    int x[2], y[2], TAngle[2], HAngle[2], port, cid, ConnectedCnt;
    bool isFiring[2], gameStarted;
    //QTcpServer *server;
    QHash<QTcpSocket*, QByteArray*> buffers; //We need a buffer to store data until block has completely received
    QHash<QTcpSocket*, qint32*> sizes;
    qint32 ArrayToInt(QByteArray);
    void dataRecieved(QByteArray);
private slots:
    //void sessionOpened();
    void newConnection();
    void disconnected();
    void readyRead();

};

#endif // SERVER_H

