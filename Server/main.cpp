#include <QCoreApplication>
#include <iostream>
#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    server s(2057, 2210, 0, "simple.map");
    std::cout << "Awaiting Connection\n";
    return a.exec();
}
