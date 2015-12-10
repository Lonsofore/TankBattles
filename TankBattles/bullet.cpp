#include "bullet.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QList>
#include "game.h"
#include "delay.h"
#include <typeinfo>

#include <math.h>
#define PI 3.14159265
#include <QDebug>

extern Game * game;

int count;
bool isfire;

Bullet::Bullet()
{
    speed = 40;
    degree = game->player->hdegree;
    pixsize = game->player->pixsize*0.16;

    image = ":/images/images/bullet.png";
    setPixmap(QPixmap(image).scaled(pixsize,pixsize));
    rotate();
    setZValue(0);

    count = 0;
    pm = new QGraphicsPixmapItem;
    game->scene->addItem(pm);

    QTimer *timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(move()));
    timer->start(20);

    QTimer *timerf = new QTimer();
    connect(timerf,SIGNAL(timeout()),this,SLOT(anim()));
    timerf->start(50);
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

QPixmap Bullet::rotatePix(QPixmap pix, int deg)
{
    QPixmap shipPixels(pix);
    QPixmap rotatePixmap(shipPixels.size());
    rotatePixmap.fill(Qt::transparent);

    QPainter p(&rotatePixmap);
    p.setRenderHint(QPainter::Antialiasing); // сглаживание
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    p.setRenderHint(QPainter::HighQualityAntialiasing);
    p.translate(rotatePixmap.size().width() / 2, rotatePixmap.size().height() / 2);
    p.rotate(deg); // градус
    p.translate(-rotatePixmap.size().width() / 2, -rotatePixmap.size().height() / 2);
    p.drawPixmap(0, 0, shipPixels);
    p.end();
    shipPixels = rotatePixmap;

    return shipPixels;
}

// анимация выстрела
void Bullet::anim()
{
    if (count >= 3 && isfire == true)
    {
        isfire = false;
        delete pm;
    }
    if (count < 3)
    {
        QPixmap pix(":/images/images/fire/" + QString::number(count) + ".png");
        QPixmap pix1;
        pix1 = rotatePix(pix,degree+90).scaled(pixsize,pixsize);

        pm->setPos(sx,sy);
        pm->setPixmap(pix1);

        isfire = true;
        count++;
    }
}

void Bullet::move()
{
    // записывание начальных координат
    if (count == 0)
    {
        sx = x();
        sy = y();
    }

    // collide
    QList<QGraphicsItem *> colliding_items = collidingItems();

    for (int i = 0, n = colliding_items.size(); i < n; ++i)
    {
        if (typeid(*(colliding_items[i])) == typeid(Tank))
        {
            Tank * tank = dynamic_cast <Tank *> (colliding_items[i]);

            if (isfire == true)
            {
                isfire = false;
                delete pm;
            }

            scene()->removeItem(this);
            delete(this);

            tank->deleteTank();
            tank->spawnTank();
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

                if (isfire == true)
                {
                    isfire = false;
                    delete pm;
                }

                scene()->removeItem(colliding_items[i]);
                scene()->removeItem(this);

                delete(colliding_items[i]);
                delete(this);

                return;
            }
            if (block->num == 2) // неразрушаемый
            {
                QMediaPlayer *sound = new QMediaPlayer();
                sound->setMedia(QUrl("qrc:/sounds/sounds/nobreak.mp3"));
                sound->setVolume(game->veffects);
                sound->play();

                if (isfire == true)
                {
                    isfire = false;
                    delete pm;
                }

                scene()->removeItem(this);
                delete(this);

                return;
            }
        }
    }   

    float x1 = x() + cos(degree * (PI / 180))*speed;
    float y1 = y() + sin(degree * (PI / 180))*speed;
    setPos(x1,y1);
    rotate();

    if (pos().y() < 0 || pos().y() > scene()->height() || pos().x() < 0 || pos().x() > scene()->width())
    {
        scene()->removeItem(this);
        delete this;
    }
}
