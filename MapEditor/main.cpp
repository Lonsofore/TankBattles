#include "mainwindow.h"
#include "preview.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    if (QApplication::arguments().size() > 1)
    {
        w.LoadMap(QString(QApplication::arguments().at(1)));
    }
    return a.exec();
}
