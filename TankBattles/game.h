#ifndef GAME
#define GAME

#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsScene>
#include "tank.h"
#include "score.h"
#include "health.h"
#include "button.h"
#include "player.h"

class Game: public QGraphicsView{
    Q_OBJECT
public:
    Game(QWidget * parent=0);
    void mouseReleaseEvent(QMouseEvent * event);
    //void keyPressEvent(QKeyEvent * event);

    void menu();
    void switchButton();
    int numsButtons; // сделано отдельно для обращения из класса button
    int curButton;

    QGraphicsScene * scene;
    Player * player;
    Score * score;
    Health * health;
public slots:
    void start();
};

#endif // GAME













