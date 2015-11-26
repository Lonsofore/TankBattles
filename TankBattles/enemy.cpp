#include "enemy.h"
#include <QTimer>
#include <QGraphicsScene>
#include "game.h"

#include <QDebug>

extern Game * game;

Enemy::Enemy()
{
    setPos(0, 0);

    setRect(0,0,100,100);

    QTimer *timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(move()));

    timer->start(50);
}

void Enemy::move()
{

    if (x() == 0 && y() != scene()->height() - 100) // вниз
        setPos(x(),y()+10);

    if (x() != scene()->width() - 100 && y() == scene()->height() - 100) // вправо
        setPos(x()+10,y());

    if (x() == scene()->width() - 100 && y() != 0) // вверх
        setPos(x(),y()-10);

    if (x() != 0 && y() == 0) // влево
        setPos(x()-10,y());

    if (x() == 0 && y() == 0)
    {
        scene()->removeItem(this);
        delete this;
        //game->health->decrease(10);
    }
}

