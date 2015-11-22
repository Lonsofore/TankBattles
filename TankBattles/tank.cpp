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

Tank::Tank(QGraphicsItem *parent)
{
    image = ":/images/images/tank1.png";
    setPixmap(QPixmap(image));
    //setPos(scene()->width()/2, scene()->height()/2);
    setPos(300,200);
    degree = 0;
    speed = 5;
    rspeed = 6;

    bulletsound = new QMediaPlayer();
    bulletsound->setMedia(QUrl("qrc:/sounds/sounds/dude.mp3"));
}

void Tank::keyPressEvent(QKeyEvent *event)
{
    int x1,y1;
    QPixmap tank(image);
    switch (event->key())
    {
        case Qt::Key_Up:
            x1 = x() + round(cos(degree * (PI / 180))*speed);
            y1 = y() + round(sin(degree * (PI / 180))*speed);
            if (y1 > 0 && y1 < scene()->height()-tank.size().height() && x1 > 0 && x1 < scene()->width()-tank.size().width())
            {
                setPos(x1,y1);
            }
            break;

        case Qt::Key_Down:
            x1 = x() - round(cos(degree * (PI / 180))*speed);
            y1 = y() - round(sin(degree * (PI / 180))*speed);
            if (y1 > 0 && y1 < scene()->height()-tank.size().height() && x1 > 0 && x1 < scene()->width()-tank.size().width())
            {
                setPos(x1,y1);
            }
            break;

        case Qt::Key_Left:
            if (degree - rspeed > -360)
                degree -= rspeed;
            else
                degree = 0;
            rotate();
            break;

        case Qt::Key_Right:
            if (degree + rspeed < 360)
                degree += rspeed;
            else
                degree = 0;
            rotate();
            break;

        case Qt::Key_Space:
            Bullet *bullet = new Bullet();

            x1 = x() + 50 + round(cos(degree * (PI / 180)) * tank.size().width()/2);  // пока писал - забыл, что такое 50...
            y1 = y() + 50 + round(sin(degree * (PI / 180)) * tank.size().height()/2); // по идее, тут центр смещается в середину танка и от него идет рассчет, где появится пуля

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

    setPixmap(shipPixels);
}

void Tank::spawn()
{
    Enemy *enemy = new Enemy();
    scene()->addItem(enemy);
}





























