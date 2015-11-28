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
#include "block.h"

class Game: public QGraphicsView{
    Q_OBJECT
public:
    Game(QWidget * parent=0);
    void mouseReleaseEvent(QMouseEvent * event);
    void changeEvent(QEvent * event);

    // меню
    void switchButton(); // сменить кнопку на выбранную
    int numsButtons; // сделано отдельно для обращения из класса button - кол-во кнопок
    int curButton; // какая кнопка выбрана в данный момент
    bool pressed;  // была ли нажата кнопка

    // сцена
    QGraphicsScene * scene;
    int xBlocks;
    int yBlocks;
    int spawns;

    // игрок
    Player * player;
    Score * score;
    Health * health;

    void moveToCenter();
    void delay(int millisecondsToWait); // функция задержки
public slots:
    void menu(); // запустить меню
    void pve();
    void pvp();
    void settings();
};

#endif // GAME













