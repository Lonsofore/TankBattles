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

    // обнуление
    xfix = 0;
    yfix = 0;

    // платформа
    setPos(x,y);
    degree = 0;
    speed = 5;
    rspeed = 3;
    baseImage = ":/images/images/greenBase.png";
    setPixmap(QPixmap(baseImage));

    // башня
    head = new QGraphicsPixmapItem();
    head->setPos(x,y);
    hdegree = 0;
    hspeed = 3;
    headImage = ":/images/images/greenHead.png";
    head->setPixmap(QPixmap(headImage));

    // звук выстрела
    bulletsound = new QMediaPlayer();
    bulletsound->setMedia(QUrl("qrc:/sounds/sounds/shot.mp3"));

    // выделить танк на сцене - для действий с ним
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();
}

void Tank::keyPressEvent(QKeyEvent *event)
{
    //qDebug() << (uint) event->key(); // чтобы узнать код клавиши
    int x1,y1;
    int xadd = 0;
    int yadd = 0;
    double sn, cs;
    QPixmap tank(baseImage);
    switch ((uint) event->key())
    {
        // Двигаться вперед
        case 16777235: // Up
            // чтобы высчитывать это только 1 раз
            cs = cos(degree * (PI / 180))*speed;
            sn = sin(degree * (PI / 180))*speed;

            // исправление кривого передвижения из-за int координат
            xfix += round(cs) - cs;
            yfix += round(sn) - sn;

            // если есть целое значение
            if (round(xfix) != 0)
            {
                xadd = round(xfix);
                xfix = xfix - round(xfix);
            }
            if (round(yfix) != 0)
            {
                yadd = round(yfix);
                yfix = yfix - round(yfix);
            }

            // новые координаты
            x1 = x() + round(cs) - xadd;
            y1 = y() + round(sn) - yadd;
            if (y1 > 0 && y1 < scene()->height()-tank.size().height() && x1 > 0 && x1 < scene()->width()-tank.size().width())
            {
                setPos(x1,y1);
                head->setPos(x1,y1);
            }
        break;

        // Двигаться назад
        case 16777237: // Down
            // чтобы высчитывать это только 1 раз
            cs = cos(degree * (PI / 180))*speed;
            sn = sin(degree * (PI / 180))*speed;

            // исправление кривого передвижения из-за int координат
            xfix -= round(cs) - cs;
            yfix -= round(sn) - sn;

            // если есть целое значение
            if (round(xfix) != 0)
            {
                xadd = round(xfix);
                xfix = xfix - round(xfix);
            }
            if (round(yfix) != 0)
            {
                yadd = round(yfix);
                yfix = yfix - round(yfix);
            }

            // новые координаты
            x1 = x() - round(cs) - xadd;
            y1 = y() - round(sn) - yadd;
            if (y1 > 0 && y1 < scene()->height()-tank.size().height() && x1 > 0 && x1 < scene()->width()-tank.size().width())
            {
                setPos(x1,y1);
                head->setPos(x1,y1);
            }
        break;

        // Поворот танка налево
        case 16777234: // Left
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

        // Поворот танка направо
        case 16777236: // Right
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

        // Поворот башни налево
        case 90:   // Z
        case 1071: // Я
            if (hdegree - hspeed > -360)
                hdegree -= hspeed;
            else
                hdegree = 0;
            hrotate();
        break;

        // Поворот башни направо
        case 88:   // X
        case 1063: // Ч
            if (hdegree + hspeed < 360)
                hdegree += hspeed;
            else
                hdegree = 0;
            hrotate();
        break;

        // Выстрел
        case 32: // SPAAAAACE
            Bullet *bullet = new Bullet();

            x1 = x() + tank.size().width()/2 - 20 + round(cos(hdegree * (PI / 180)) * tank.size().width()/2);   // понятия не имею, что за 20
            y1 = y() + tank.size().height()/2 - 20 + round(sin(hdegree * (PI / 180)) * tank.size().height()/2); // я его просто подобрал
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

void Tank::rotate() // поворот платформы
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

void Tank::hrotate() // поворот башни
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





























