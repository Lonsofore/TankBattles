#include <QApplication>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QIcon>
#include "game.h"

Game * game;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::addLibraryPath("./");
    QApplication::setWindowIcon(QIcon(":/images/images/icon.ico"));

    game = new Game();
    game->show();
    game->menu();

    return a.exec();
}
