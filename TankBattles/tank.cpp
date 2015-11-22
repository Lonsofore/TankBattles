#include "tank.h"
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "bullet.h"
#include "enemy.h"
#include "game.h"

#include <math.h>
#define PI 3.14159265

#include <QDebug>

extern Game * game;

Tank::Tank()
{
    int x = 300;
    int y = 200;

    setPos(x,y);
    degree = 0;
    speed = 5;
    rspeed = 6;

    baseImage = ":/images/images/greenBase.png";
    setPixmap(QPixmap(baseImage));

    head = new QGraphicsPixmapItem();
    head->setPos(x,y);
    hdegree = 0;
    hspeed = 6;
    headImage = ":/images/images/greenHead.png";
    head->setPixmap(QPixmap(headImage));

    bulletsound = new QMediaPlayer();
    bulletsound->setMedia(QUrl("qrc:/sounds/sounds/shot.mp3"));

    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();
}

void Tank::keyPressEvent(QKeyEvent *event)
{
    int x1,y1;
    QPixmap tank(baseImage);
    switch (event->key())
    {
        case Qt::Key_Up:
            x1 = x() + round(cos(degree * (PI / 180))*speed);
            y1 = y() + round(sin(degree * (PI / 180))*speed);
            if (y1 > 0 && y1 < scene()->height()-tank.size().height() && x1 > 0 && x1 < scene()->width()-tank.size().width())
            {
                setPos(x1,y1);
                head->setPos(x1,y1);
            }
        break;

        case Qt::Key_Down:
            x1 = x() - round(cos(degree * (PI / 180))*speed);
            y1 = y() - round(sin(degree * (PI / 180))*speed);
            if (y1 > 0 && y1 < scene()->height()-tank.size().height() && x1 > 0 && x1 < scene()->width()-tank.size().width())
            {
                setPos(x1,y1);
                head->setPos(x1,y1);
            }
        break;

        case Qt::Key_Left:
            if (degree - rspeed > -360)
                degree -= rspeed;
            else
                degree = 0;
            rotate();

            if (hdegree - rspeed > -360)
                hdegree -= rspeed;
            else
                hdegree = 0;
            hrotate();
        break;

        case Qt::Key_Right:
            if (degree + rspeed < 360)
                degree += rspeed;
            else
                degree = 0;
            rotate();

            if (hdegree + rspeed < 360)
                hdegree += rspeed;
            else
                hdegree = 0;
            hrotate();
        break;

        case Qt::Key_Z:
            if (hdegree - hspeed > -360)
                hdegree -= hspeed;
            else
                hdegree = 0;
            hrotate();
        break;

        case Qt::Key_X:
            if (hdegree + hspeed < 360)
                hdegree += hspeed;
            else
                hdegree = 0;
            hrotate();
        break;

        case Qt::Key_Space:
            Bullet *bullet = new Bullet();

            x1 = x() + tank.size().width()/2 - 20 + round(cos(hdegree * (PI / 180)) * tank.size().width()/2);  // понятия не имею, что за 20
            y1 = y() + tank.size().height()/2 - 20 + round(sin(hdegree * (PI / 180)) * tank.size().height()/2);// я его просто подобрал

            //bullet->setPos(x()+48,y());
            bullet->setPos(x1,y1);

            scene()->addItem(bullet);

            switch (bulletsound->state())
            {
                case QMediaPlayer::PlayingState:
                    bulletsound->setPosition(0);
                    break;
                case QMediaPlayer::StoppedState:
                    bulletsound->play();
                    break;
            }

        break;
    }
    //game->health->setPos(game->player->x()+40,game->player->y()+50);
}

void Tank::rotate()
{
    QPixmap shipPixels(baseImage);
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

    setPixmap(shipPixels);
}

void Tank::hrotate()
{
    QPixmap shipPixels(headImage);
    QPixmap rotatePixmap(shipPixels.size());
    rotatePixmap.fill(Qt::transparent);

    QPainter p(&rotatePixmap);
    p.setRenderHint(QPainter::Antialiasing); // сглаживание
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    p.setRenderHint(QPainter::HighQualityAntialiasing);
    p.translate(rotatePixmap.size().width() / 2, rotatePixmap.size().height() / 2);
    p.rotate(hdegree); // градус
    p.translate(-rotatePixmap.size().width() / 2, -rotatePixmap.size().height() / 2);
    p.drawPixmap(0, 0, shipPixels);
    p.end();
    shipPixels = rotatePixmap;

    head->setPixmap(shipPixels);
}

void Tank::spawn()
{
    Enemy *enemy = new Enemy();
    scene()->addItem(enemy);
}





























