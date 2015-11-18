#ifndef GAME
#define GAME

#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsScene>
#include "Tank.h"
#include "Score.h"
#include "Health.h"

class Game: public QGraphicsView{
public:
    Game(QWidget * parent=0);

    QGraphicsScene * scene;
    Tank * player;
    Score * score;
    Health * health;

};

#endif // GAME

