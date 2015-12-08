#include <QApplication>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "game.h"

Game * game;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::addLibraryPath("./");

    game = new Game();
    game->show();
    game->menu();

    return a.exec();
}
