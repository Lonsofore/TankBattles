#include <QCoreApplication>
#include <iostream>
#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    server s(7, 0);
    std::cout << "Awaiting Connection\n";
    return a.exec();
}
