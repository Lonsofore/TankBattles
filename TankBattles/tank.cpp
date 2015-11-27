#include "tank.h"
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "bullet.h"
#include "enemy.h"
#include "game.h"
#include <QTime>
#include <QCoreApplication>

#include <math.h>
#define PI 3.14159265

#include <QDebug>

extern Game * game;

Tank::Tank()
{
    int x = 300;
    int y = 200;

    // отдельная функция для того, чтобы ее вызывать классом player
    defaultTank(x,y);
}

void Tank::defaultTank(int x, int y)
{
    pixsize = 120;

    // обнуление
    xfix = 0;
    yfix = 0;

    // платформа
    setPos(x,y);
    degree = 0;
    speed = 2;
    rspeed = 2;
    baseImage = ":/images/images/tanks/greenBase.png";
    setPixmap(QPixmap(baseImage).scaled(pixsize,pixsize));

    // башня
    head = new QGraphicsPixmapItem();
    head->setPos(x,y);
    hdegree = 0;
    hspeed = 2;
    headImage = ":/images/images/tanks/greenHead.png";
    head->setPixmap(QPixmap(headImage).scaled(pixsize,pixsize));

    // звук выстрела
    bulletsound = new QMediaPlayer();
    bulletsound->setMedia(QUrl("qrc:/sounds/sounds/tank_fire.mp3"));

    //звук перезарядки
    bulletready = new QMediaPlayer();
    bulletready->setMedia(QUrl("qrc:/sounds/sounds/tank_reload.mp3"));

    //звук поворота башни
    tankhrotate = new QMediaPlayer();
    tankhrotate->setMedia(QUrl("qrc:/sounds/sounds/tank_hrotate.wav"));
}



void Tank::moveForward()
{
    int x1,y1;
    int xadd = 0;
    int yadd = 0;
    double sn, cs;
    QPixmap tank(baseImage);
    tank.scaled(pixsize,pixsize);

    // чтобы высчитывать это только 1 раз
    cs = cos(degree * (PI / 180))*speed;
    sn = sin(degree * (PI / 180))*speed;

    // исправление кривого передвижения из-за int координат
    xfix = xfix + round(cs) - cs;
    yfix = yfix + round(sn) - sn;

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

    int height = this->pixmap().size().height();
    int width = this->pixmap().size().width();

    // новые координаты
    x1 = x() + round(cs) - xadd;
    y1 = y() + round(sn) - yadd;
    if (y1 > 0 && y1 < scene()->height()-height && x1 > 0 && x1 < scene()->width()-width)
    {
        setPos(x1,y1);
        head->setPos(x1,y1);
    }
}

void Tank::moveBack()
{
    int x1,y1;
    int xadd = 0;
    int yadd = 0;
    double sn, cs;

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

    int height = this->pixmap().size().height();
    int width = this->pixmap().size().width();

    // новые координаты
    x1 = x() - round(cs) - xadd;
    y1 = y() - round(sn) - yadd;
    if (y1 > 0 && y1 < scene()->height()-height && x1 > 0 && x1 < scene()->width()-width)
    {
        setPos(x1,y1);
        head->setPos(x1,y1);
    }
}

void Tank::rotateRight()
{
    int deg = rspeed;
    degree += deg;
    if (degree > 360 || degree < -360)
        degree = 0;
    rotate();

    hdegree += deg;
    if (hdegree > 360 || hdegree < -360)
        hdegree = 0;
    hrotate();
}

void Tank::rotateLeft()
{
    int deg = rspeed;
    degree -= deg;
    if (degree > 360 || degree < -360)
        degree = 0;
    rotate();

    hdegree -= deg;
    if (hdegree > 360 || hdegree < -360)
        hdegree = 0;
    hrotate();
}

void Tank::rotateRight(int deg)
{
    degree += deg;
    if (degree > 360 || degree < -360)
        degree = 0;
    rotate();

    hdegree += deg;
    if (hdegree > 360 || hdegree < -360)
        hdegree = 0;
    hrotate();
}

void Tank::rotateLeft(int deg)
{
    degree -= deg;
    if (degree > 360 || degree < -360)
        degree = 0;
    rotate();

    hdegree -= deg;
    if (hdegree > 360 || hdegree < -360)
        hdegree = 0;
    hrotate();
}

void Tank::headRight()
{
    if (hdegree + hspeed < 360)
        hdegree += hspeed;
    else
        hdegree = 0;
    hrotate();
}

void Tank::headLeft()
{
    if (hdegree - hspeed > -360)
        hdegree -= hspeed;
    else
        hdegree = 0;
    hrotate();
}

void Tank::fire()
{
    int x1,y1;
    x1 = x() + pixsize/2 - 20 + round(cos(hdegree * (PI / 180)) * pixsize/2);   // понятия не имею, что за 20
    y1 = y() + pixsize/2 - 20 + round(sin(hdegree * (PI / 180)) * pixsize/2); // я его просто подобрал

    Bullet *bullet = new Bullet();
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
}

// с возвратом qpixmap какой-то гемор, потому функций сделано две
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

    setPixmap(shipPixels.scaled(pixsize,pixsize));
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

    head->setPixmap(shipPixels.scaled(pixsize,pixsize));
}

void Tank::delay( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

























