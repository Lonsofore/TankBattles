#ifndef GAME
#define GAME

#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsScene>
#include "tank.h"
#include "score.h"
#include "health.h"

class Game: public QGraphicsView{
public:
    Game(QWidget * parent=0);

    QGraphicsScene * scene;
    Tank * player;
    Score * score;
    Health * health;

};

#endif // GAME

