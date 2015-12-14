#include "bullet.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QList>
#include "game.h"
#include "delay.h"
#include <typeinfo>
#include <QLabel>
#include <QMovie>

#include <math.h>
#define PI 3.14159265

#include <QDebug>

int Round(double x);

extern Game * game;

int count;
bool isfire;

Bullet::Bullet(QGraphicsPixmapItem *parent)
{
    // родительский танк
    tank = new Tank;
    tank = dynamic_cast <Tank *> (parent);

    dmg = 40;

    speed = 40;
    degree = tank->hdegree;
    pixsize = tank->pixsize*0.16;

    image = ":/images/images/bullet.png";
    empty = ":/images/images/empty.png";
    setPixmap(QPixmap(image).scaled(pixsize,pixsize));
    rotate();
    setZValue(0);

    count = 0;
    pm = new QGraphicsPixmapItem;
    game->scene->addItem(pm);

    timerf = new QTimer();
    connect(timerf,SIGNAL(timeout()),this,SLOT(anim()));
    timerf->start(50);

    timer = new QTimer();
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
    if (count >= 3)
    {
        delete pm;
    }

    if (count < 3)
    {
        isfire = true;

        QPixmap pix(":/images/images/fire/" + QString::number(count) + ".png");
        QPixmap pix1;
        pix1 = rotatePix(pix,degree+90).scaled(pixsize*1.2,pixsize*1.2);

        int x1 = tank->x() + tank->pixsize/2 + Round(cos(tank->hdegree * PI / 180) * (tank->pixsize/2 + tank->pixsize/20)) - pixsize*1.2/2;
        int y1 = tank->y() + tank->pixsize/2 + Round(sin(tank->hdegree * PI / 180) * (tank->pixsize/2 + tank->pixsize/20)) - pixsize*1.2/2;

        pm->setPos(x1,y1);
        pm->setPixmap(pix1);

        count++;
    }
    else
    {
        if (timer->isActive() == false)
        {
            scene()->removeItem(this);
            delete(this);
        }
        else
            timerf->stop();
    }
}

void Bullet::move()
{
    // collide
    QList<QGraphicsItem *> colliding_items = collidingItems();

    bool hit = false;

    for (int i = 0, n = colliding_items.size(); i < n; ++i)
    {
        if (typeid(*(colliding_items[i])) == typeid(Tank))
        {
            hit = true;

            Tank * tank1 = dynamic_cast <Tank *> (colliding_items[i]);
            tank1->decHealth(dmg);

            /*
            QLabel *gif_anim = new QLabel();
            gif_anim->setStyleSheet("background-color: rgba(229, 229, 229, 10);");
            QMovie *movie = new QMovie(":/images/images/anim/Explo.gif");
            gif_anim->setMovie(movie);
            gif_anim->move(x()-25,y()-25);
            movie->setScaledSize(QSize(250,250));
            movie->start();
            QGraphicsProxyWidget *proxy = game->scene->addWidget(gif_anim);
            */
        }

        if (typeid(*(colliding_items[i])) == typeid(Block))
        {
            Block * block = dynamic_cast <Block *> (colliding_items[i]);
            if (block->num == 1) // разрушаемый
            {
                hit = true;

                int random = 1 + rand()%3;
                QMediaPlayer *sound = new QMediaPlayer();
                sound->setMedia(QUrl("qrc:/sounds/sounds/break" + QString::number(random) + ".mp3"));
                sound->setVolume(game->veffects);
                sound->play();

                scene()->removeItem(colliding_items[i]);
                delete(colliding_items[i]);
            }
            if (block->num == 2) // неразрушаемый
            {
                hit = true;

                QMediaPlayer *sound = new QMediaPlayer();
                sound->setMedia(QUrl("qrc:/sounds/sounds/nobreak.mp3"));
                sound->setVolume(game->veffects);
                sound->play();
            }
        }

        if (hit)
        {
            if (timerf->isActive() == false && isfire == true)
            {
                scene()->removeItem(this);
                delete(this);
            }
            else
            {
                setPixmap(QPixmap(empty));
                timer->stop();
            }
            return;
        }
    }   

    float x1 = x() + cos(degree * (PI / 180))*speed;
    float y1 = y() + sin(degree * (PI / 180))*speed;
    setPos(x1,y1);
    rotate();

    if (pos().y() < 0 || pos().y() > scene()->height() || pos().x() < 0 || pos().x() > scene()->width())
    {
        if (timerf->isActive() == false && isfire == true)
        {
            scene()->removeItem(this);
            delete(this);
        }
        else
        {
            setPixmap(QPixmap(empty));
            timer->stop();
        }
    }
}

int Round(double x)
{
   if (x > 0)
   {
       x += 0.5;
       return (int)x;
   }
   if (x < 0)
   {
       x -= 0.5;
       return (int)x;
   }
   if (x == 0) return 0;
}
