#include <QCoreApplication>
#include <QtNetwork>
#include <iostream>
#include "server.h"
static inline QByteArray IntToArray(qint32 source);
server::server(int port1, bool isStartdByClient)
{   tcpServer = new QTcpServer();
    udpSocket = new QUdpSocket();
    port = port1;
    gameStarted = 0;
    ConnectedCnt = 0;
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
            tcpServer->listen(QHostAddress::Any, port);
}

void server::broadcastDatagram()
{
    std::cout << "SENDING: X=" << x[cid] << ",Y=" << y[cid] << " FROM CLIENT # " << cid << "\n";
    QByteArray datagram = QByteArray::number(0) + ' ' + QByteArray::number(x[0]) + ' ' + QByteArray::number(y[0])+ ' ' +
            QByteArray::number(TAngle[0]) + ' ' + QByteArray::number(HAngle[0]) + ' ' + QByteArray::number(isFiring[0]) + '|';
       datagram += QByteArray::number(1) + ' ' + QByteArray::number(x[1]) + ' ' + QByteArray::number(y[1])+ ' ' +
               QByteArray::number(TAngle[1]) + ' ' + QByteArray::number(HAngle[1]) + ' ' + QByteArray::number(isFiring[1]) +  '|';
    udpSocket->writeDatagram(datagram.data(), datagram.size(),
                             QHostAddress::Broadcast, 45454);
}

void server::dataRecieved(QByteArray data)
{
    if (ConnectedCnt >= 2) gameStarted = 1;
    if (gameStarted)
    {
        QList<QByteArray> list;
        list = data.split(' ');
        cid = list.at(0).toInt();
        x[cid] = list.at(1).toInt();
        y[cid] = list.at(2).toInt();
        TAngle[cid] = list.at(3).toInt();
        HAngle[cid] = list.at(4).toInt();
        isFiring[cid] = list.at(5).toInt();
        broadcastDatagram();
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
        if (!gameStarted)
        {
            QByteArray data;
            if (ConnectedCnt >= 2)
            {
                data = "FULL";
            }
            else
            {
                data = QByteArray::number(ConnectedCnt++);
            }
            socket->write(IntToArray(data.size()));
            socket->write(data);
            socket->waitForBytesWritten();
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
