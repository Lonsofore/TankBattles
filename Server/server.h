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
    server(int tport, int uport, bool isStartdByClient, QString rmap, int max_clients);
private:
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    QNetworkSession *networkSession;
    QUdpSocket *udpSocket;
    void broadcastDatagram(); //Рассылка данных
    int x[16], y[16], TAngle[16], HAngle[16], health[16], tcpPort, udpPort, cid, ConnectedCnt, DisConnCnt; //Данные об игроках, порты, id текущего игрока, кол-во подключ. и отключ. игроков
    bool isFiring[16], gameStarted; //Стрельба Игра_началась
    int max_cl;
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

