#include <QCoreApplication>
#include <QtNetwork>
#include <iostream>
#include "server.h"
QString GroupIP = "255.255.255.255";
static inline QByteArray IntToArray(qint32 source);
server::server(int tport, int uport, bool isStartdByClient, QString rmap, int max_clients)
{   tcpServer = new QTcpServer();
    udpSocket = new QUdpSocket();
    tcpPort = tport;
    udpPort = uport;
    max_cl = max_clients;
    gameStarted = false;
    ConnectedCnt = 0;
    DisConnCnt = 0;
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
    tcpServer->listen(QHostAddress::Any, tcpPort);
    for (int i = 0; i < max_cl; i++)
    {
        x[i] = 0;
        y[i] = 0;
        TAngle[i] = 0;
        HAngle[i] = 0;
        isFiring[i] = 0;
    }
    map = rmap;
}

void server::broadcastDatagram()
{
    std::cout << "SENDING: X=" << x[cid] << ",Y=" << y[cid] << " FROM CLIENT # " << cid << "\n";
    QByteArray datagram;
    for (int i = 0; i < max_cl; i++)
    {
        datagram += QByteArray::number(i) + ' ' + QByteArray::number(x[i]) + ' ' + QByteArray::number(y[i])+ ' ' +
                       QByteArray::number(TAngle[i]) + ' ' + QByteArray::number(HAngle[i]) + ' ' + QByteArray::number(isFiring[i]) +
                       ' ' + QByteArray::number(health[i]) + '|';
    }
    udpSocket->writeDatagram(datagram.data(), datagram.size(),
                             QHostAddress(GroupIP), udpPort);
}

void server::dataRecieved(QByteArray data)
{
    //if (ConnectedCnt >= max_cl && DisConnCnt != ConnectedCnt) gameStarted = 1;
    QList<QByteArray> list;
    list = data.split(' ');
    cid = list.at(0).toInt();
    if (list.at(1) != "SYSTEM") //Если не системное сообщение
    {
        x[cid] = list.at(1).toInt(); //Заносим данные
        y[cid] = list.at(2).toInt();
        TAngle[cid] = list.at(3).toInt();
        HAngle[cid] = list.at(4).toInt();
        isFiring[cid] = list.at(5).toInt();
        health[cid] = list.at(6).toInt();
        if (gameStarted) broadcastDatagram(); //Если игра идет, то отправляем данные
    }
    else
    {
        if (list.at(2) == "EXIT")
        {
            QByteArray datagram = QByteArray::number(cid) + " SYSTEM EXIT";
            udpSocket->writeDatagram(datagram.data(), datagram.size(),
                                     QHostAddress(GroupIP), udpPort);
            DisConnCnt++;
            if (ConnectedCnt == DisConnCnt)
            {
                ConnectedCnt = 0;
                DisConnCnt = 0;
                for (int i = 0; i < max_cl; i++)
                {
                    x[i] = 0;
                    y[i] = 0;
                    TAngle[i] = 0;
                    HAngle[i] = 0;
                    isFiring[i] = 0;
                }
            } //Если все отключились, то обнуляем сервер
        }
        if (list.at(2) == "START")
        {
            gameStarted = 1;
            QByteArray datagram = QByteArray::number(ConnectedCnt) + ' ' + "SYSTEM STARTED";
            udpSocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress(GroupIP), udpPort);
        }
    }
}

void server::newConnection()
{
    while (tcpServer->hasPendingConnections())
    {
        QTcpSocket *socket = tcpServer->nextPendingConnection();
        connect(socket, SIGNAL(readyRead()), SLOT(readyRead()));
        connect(socket, SIGNAL(disconnected()), SLOT(disconnected()));
        std::cout << "CONNECTED: " << socket->peerAddress().toString().toStdString() << std::endl;
        QByteArray *buffer = new QByteArray();
        qint32 *s = new qint32(0);
        buffers.insert(socket, buffer);
        sizes.insert(socket, s);
        QByteArray data;
        if (!gameStarted) //Если игра не началась
        {

            if (ConnectedCnt >= max_cl)
            {
                data = "FULL"; //Если сервер переполнен
            }
            else
            {
                data = QByteArray::number(ConnectedCnt++) + ' ' + QByteArray::number(udpPort) + ' ' + map.toLatin1().constData() + ' ' + QByteArray::number(max_cl); //Отправляем приветствие
                //Формат: id udpПорт Карта Число_игроков
                QByteArray datagram = QByteArray::number(ConnectedCnt) + ' ' + "SYSTEM CONNECTED";
                udpSocket->writeDatagram(datagram.data(), datagram.size(),
                                             QHostAddress(GroupIP), udpPort);
            }
        }
        else
        {
            data = "STARTED"; //Игра уже началась
        }
        socket->write(IntToArray(data.size()));
        socket->write(data);
        socket->waitForBytesWritten();
        if (!gameStarted)
        {
            if (ConnectedCnt >= max_cl && DisConnCnt != ConnectedCnt)
            {
                gameStarted = 1;
                QByteArray datagram = QByteArray::number(ConnectedCnt) + ' ' + "SYSTEM STARTED";
                udpSocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress(GroupIP), udpPort);
            }
        }
        if (gameStarted)
        {
            QByteArray datagram = QByteArray::number(ConnectedCnt) + ' ' + "SYSTEM STARTED";
            udpSocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress(GroupIP), udpPort);
        }
    }
}
void server::disconnected()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    QByteArray *buffer = buffers.value(socket);
    qint32 *s = sizes.value(socket);
    socket->deleteLater();
    delete buffer;
    delete s;
}

void server::readyRead()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    QByteArray *buffer = buffers.value(socket);
    qint32 *s = sizes.value(socket);
    qint32 size = *s;
    while (socket->bytesAvailable() > 0)
    {
        buffer->append(socket->readAll());
        while ((size == 0 && buffer->size() >= 4) || (size > 0 && buffer->size() >= size)) //Пока есть данные
        {
            if (size == 0 && buffer->size() >= 4) //Если получен размер, сохраняем его
            {
                size = ArrayToInt(buffer->mid(0, 4));
                *s = size;
                buffer->remove(0, 4);
            }
            if (size > 0 && buffer->size() >= size) // Если все данные получены, от обрабатываем их
            {
                QByteArray data = buffer->mid(0, size);
                buffer->remove(0, size);
                size = 0;
                *s = size;
                dataRecieved(data);
            }
        }
    }
}

qint32 server::ArrayToInt(QByteArray source) //Конвертирование из QByteArray в int
{
    qint32 temp;
    QDataStream data(&source, QIODevice::ReadWrite);
    data >> temp;
    return temp;
}

QByteArray IntToArray(qint32 source) //Конвертирование из int в QByteArray
{
    QByteArray temp;
    QDataStream data(&temp, QIODevice::ReadWrite);
    data << source;
    return temp;
}
