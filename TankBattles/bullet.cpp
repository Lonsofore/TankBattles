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
    speed = game->player->bulletSpeed;
    degree = game->player->hdegree;
    pixsize = game->player->pixsize*0.16;

    image = ":/images/images/bullet.png";
    setPixmap(QPixmap(image).scaled(pixsize,pixsize));
    rotate();
    setZValue(0);

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
    p.rotate(degree); // градус
    p.translate(-rotatePixmap.size().width() / 2, -rotatePixmap.size().height() / 2);
    p.drawPixmap(0, 0, shipPixels);
    p.end();
    shipPixels = rotatePixmap;

    setPixmap(shipPixels.scaled(pixsize,pixsize));
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
        if (typeid(*(colliding_items[i])) == typeid(Block))
        {
            Block * block = dynamic_cast <Block *> (colliding_items[i]);
            if (block->num == 1) // разрушаемый
            {
                int random = 1 + rand()%3;
                QMediaPlayer *sound = new QMediaPlayer();
                sound->setMedia(QUrl("qrc:/sounds/sounds/break" + QString::number(random) + ".mp3"));
                sound->setVolume(game->veffects);
                sound->play();

                scene()->removeItem(colliding_items[i]);
                scene()->removeItem(this);
                delete(colliding_items[i]);
                delete(this);
                return;
            }
            else
            {
                QMediaPlayer *sound = new QMediaPlayer();
                sound->setMedia(QUrl("qrc:/sounds/sounds/nobreak.mp3"));
                sound->setVolume(game->veffects);
                sound->play();

                scene()->removeItem(this);
                delete(this);
                return;
            }
        }
    }

    int x1 = x() + cos(degree * (PI / 180))*speed;
    int y1 = y() + sin(degree * (PI / 180))*speed;
    setPos(x1,y1);
    rotate();
    if (pos().y() < 0 || pos().y() > scene()->height() || pos().x() < 0 || pos().x() > scene()->width())
    {
        scene()->removeItem(this);
        delete this;
    }
}
