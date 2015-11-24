#ifndef GAME
#define GAME

#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsScene>
#include "tank.h"
#include "score.h"
#include "health.h"

class Game: public QGraphicsView{
    Q_OBJECT
public:
    Game(QWidget * parent=0);
    void mouseReleaseEvent(QMouseEvent * event);

    void menu();

    QGraphicsScene * scene;
    Tank * player;
    Score * score;
    Health * health;
public slots:
    void start();
};

#endif // GAME













