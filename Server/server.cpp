#include <QCoreApplication>
#include <QtNetwork>
#include <iostream>
#include "server.h"

server::server(int port1)
{   tcpServer = new QTcpServer();
    udpSocket = new QUdpSocket();
    port = port1;
    /*QNetworkConfigurationManager manager;
        if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired)
        {
            // Get saved network configuration
            QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
            settings.beginGroup(QLatin1String("QtNetwork"));
            const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
            settings.endGroup();

            // If the saved network configuration is not currently discovered use the system default
            QNetworkConfiguration config = manager.configurationFromIdentifier(id);
            if ((config.state() & QNetworkConfiguration::Discovered) !=
                QNetworkConfiguration::Discovered) {
                config = manager.defaultConfiguration();
            }

            networkSession = new QNetworkSession(config, this);
            connect(networkSession, SIGNAL(opened()), this, SLOT(sessionOpened()));
            networkSession->open();
        } /*else {
            server(port);*/
        //}
            connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
            tcpServer->listen(QHostAddress::Any, port);
}

void server::broadcastDatagram()
{
    std::cout << "SENDING: X=" << x << ",Y=" << y << " FROM CLIENT # " << cid << "\n";
    QByteArray datagram = QByteArray::number(0) + ' ' + QByteArray::number(x[0]) + ' ' + QByteArray::number(y[0])+ ' ' +
            QByteArray::number(TAngle[0]) + ' ' + QByteArray::number(HAngle[0]) +  '|';
       datagram += QByteArray::number(1) + ' ' + QByteArray::number(x[1]) + ' ' + QByteArray::number(y[1])+ ' ' +
               QByteArray::number(TAngle[1]) + ' ' + QByteArray::number(HAngle[1]) +  '|';
    udpSocket->writeDatagram(datagram.data(), datagram.size(),
                             QHostAddress::Broadcast, 45454);
}

void server::dataRecieved(QByteArray data)
{
    /*QByteArray datagram;
            QString xval, yval; //????????!!1111
            datagram.resize(udpSocket->pendingDatagramSize());
            udpSocket->readDatagram(datagram.data(), datagram.size());
            QList<QByteArray> list;
            list = datagram.split(' ');
            //xval = datagram.at(1);
            //yval = datagram.at(3);
            enmy->changePos(list.at(0).toInt(),list.at(1).toInt());*/
    QList<QByteArray> list;
    list = data.split(' ');
    cid = list.at(0).toInt();
    x[cid] = list.at(1).toInt();
    y[cid] = list.at(2).toInt();
    TAngle[cid] = list.at(3).toInt();
    HAngle[cid] = list.at(4).toInt();
    broadcastDatagram();
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
        while ((size == 0 && buffer->size() >= 4) || (size > 0 && buffer->size() >= size)) //While can process data, process it
        {
            if (size == 0 && buffer->size() >= 4) //if size of data has received completely, then store it on our global variable
            {
                size = ArrayToInt(buffer->mid(0, 4));
                *s = size;
                buffer->remove(0, 4);
            }
            if (size > 0 && buffer->size() >= size) // If data has received completely, then emit our SIGNAL with the data
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

qint32 server::ArrayToInt(QByteArray source)
{
    qint32 temp;
    QDataStream data(&source, QIODevice::ReadWrite);
    data >> temp;
    return temp;
}


