#include <QCoreApplication>
#include <iostream>
#include "server.h"

int main(int argc, char *argv[])
{
    int tcpPort = 2057;
    int udpPort = 2210;
    bool isstrd = 0;
    QString map = "simple.map";
    int max_clients = 2;
    QCoreApplication a(argc, argv);
    if (QCoreApplication::arguments().size() == 5)
    {
        tcpPort = QString(QCoreApplication::arguments().at(1)).toInt();
        udpPort = QString(QCoreApplication::arguments().at(2)).toInt();
        map = QString(QCoreApplication::arguments().at(3));
        max_clients = QString(QCoreApplication::arguments().at(4)).toInt();
        isstrd = 1;
    }
    server s(tcpPort, udpPort, isstrd, map, max_clients);
    std::cout << "Awaiting Connection\n";
    return a.exec();
}
