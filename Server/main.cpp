#include <QCoreApplication>
#include <iostream>
#include "server.h"

int main(int argc, char *argv[])
{
    int tcpPort = 2057;
    int udpPort = 2210;
    bool isstrd = 0;
    QString map = "simple.map";
    QCoreApplication a(argc, argv);
    if (QCoreApplication::arguments().size() > 3)
    {
        tcpPort = QString(QCoreApplication::arguments().at(1)).toInt();
        udpPort = QString(QCoreApplication::arguments().at(2)).toInt();
        map = QString(QCoreApplication::arguments().at(3));
        isstrd = 1;
    }
    server s(tcpPort, udpPort, isstrd, map);
    std::cout << "Awaiting Connection\n";
    return a.exec();
}
