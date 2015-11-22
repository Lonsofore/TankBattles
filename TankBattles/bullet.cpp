#include "bullet.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QList>
#include "enemy.h"
#include "game.h"
#include <typeinfo>
#include <math.h>
#define PI 3.14159265
#include <QDebug>

extern Game * game;

Bullet::Bullet()
{
    speed = 40;
    image = ":/images/images/bullet.png";
    setPixmap(QPixmap(image));
    rotate();

    QTimer *timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(move()));

    timer->start(20);
}

void Bullet::rotate()
{
    QPixmap shipPixels(image);
    QPixmap rotatePixmap(shipPixels.size());
    rotatePixmap.fill(Qt::transparent);

    QPainter p(&rotatePixmap);
    p.setRenderHint(QPainter::Antialiasing); // сглаживание
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    p.setRenderHint(QPainter::HighQualityAntialiasing);
    p.translate(rotatePixmap.size().width() / 2, rotatePixmap.size().height() / 2);
    p.rotate(game->player->hdegree); // градус
    p.translate(-rotatePixmap.size().width() / 2, -rotatePixmap.size().height() / 2);
    p.drawPixmap(0, 0, shipPixels);
    p.end();
    shipPixels = rotatePixmap;

    setPixmap(shipPixels);
}

void Bullet::move()
{
    // collide
    QList<QGraphicsItem *> colliding_items = collidingItems();

    for (int i = 0, n = colliding_items.size(); i < n; ++i)
    {
        if (typeid(*(colliding_items[i])) == typeid(Enemy))
        {
            game->score->increase();
            scene()->removeItem(colliding_items[i]);
            scene()->removeItem(this);
            delete(colliding_items[i]);
            delete(this);
            return;
        }
    }

    int deg = game->player->hdegree;
    int x1 = x() + cos(deg * (PI / 180))*speed;
    int y1 = y() + sin(deg * (PI / 180))*speed;
    setPos(x1,y1);
    rotate();
    if (pos().y() < 0 || pos().y() > scene()->height() || pos().x() < 0 || pos().x() > scene()->width())
    {
        scene()->removeItem(this);
        delete this;
    }
}
