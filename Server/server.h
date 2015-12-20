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
    server(int tport, int uport, bool isStartdByClient, QString rmap);
private:
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    QNetworkSession *networkSession;
    QUdpSocket *udpSocket;
    void broadcastDatagram(); //Рассылка данных
    int x[2], y[2], TAngle[2], HAngle[2], health[2], tcpPort, udpPort, cid, ConnectedCnt, DisConnCnt; //Данные об игроках, порты, id текущего игрока, кол-во подключ. и отключ. игроков
    bool isFiring[2], gameStarted; //Стрельба Игра_началась
    QString map; //Карта
    QHash<QTcpSocket*, QByteArray*> buffers; //Буфер для данных
    QHash<QTcpSocket*, qint32*> sizes;
    qint32 ArrayToInt(QByteArray);
    void dataRecieved(QByteArray); //Обработка данных
private slots:
    //void sessionOpened();
    void newConnection(); //Подключился новый игрок
    void disconnected(); //Отключение
    void readyRead(); //Чтение данных

};

#endif // SERVER_H

