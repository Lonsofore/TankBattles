#include "tank.h"
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "bullet.h"
#include "enemy.h"
#include "game.h"
#include <QTime>
#include <QCoreApplication>
#include <typeinfo>
#include <cstdlib>

#include <math.h>
#define PI 3.14159265

#include <QDebug>

extern Game * game;

Tank::Tank()
{
    // отдельная функция для того, чтобы ее вызывать классом player
    defaultTank();
}

void Tank::defaultTank()
{
    // размер танка
    pixsize = 200;

    // обнуление
    xfix = 0;
    yfix = 0;

    // платформа
    degree = 0;
    speed = 2;
    rspeed = 2;
    baseImage = ":/images/images/tanks/greenBase.png";
    setPixmap(QPixmap(baseImage).scaled(pixsize,pixsize));
    //setPos(x,y);
    setZValue(2); // "высота" платформы

    // башня
    head = new QGraphicsPixmapItem();
    hdegree = 0;
    hspeed = 2;
    headImage = ":/images/images/tanks/greenHead.png";
    head->setPixmap(QPixmap(headImage).scaled(pixsize,pixsize));
    //head->setPos(x,y);
    head->setZValue(3);

    //точка спавна
    randomSpawn();

    // звук выстрела
    bulletsound = new QMediaPlayer();
    bulletsound->setMedia(QUrl("qrc:/sounds/sounds/tank_fire.mp3"));
    bulletsound->setVolume(game->veffects);

    //звук перезарядки
    bulletready = new QMediaPlayer();
    bulletready->setMedia(QUrl("qrc:/sounds/sounds/tank_reload.mp3"));
    bulletready->setVolume(game->veffects);

    //звук поворота башни
    //playlist = new QMediaPlaylist;
    //playlist->addMedia(QUrl("qrc:/sounds/sounds/tank_hrotate.wav"));

    tankhrotate = new QMediaPlayer();
    tankhrotate->setMedia(QUrl("qrc:/sounds/sounds/tank_hrotate.wav"));
    tankhrotate->setVolume(game->veffects);
}



void Tank::moveForward(bool check)
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

        if (check != true)
        {
            while (isCollide() == true)
                moveBack(check = true);
        }
    }
}

void Tank::moveBack(bool check)
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

        if (check != true)
        {
            while (isCollide() == true)
                moveForward(check = true);
        }
    }
}

void Tank::rotateRight(bool check)
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

    if (check != true)
    {
        while (isCollide() == true)
            rotateLeft(check = true);
    }
}

void Tank::rotateLeft(bool check)
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

    if (check != true)
    {
        while (isCollide() == true)
            rotateRight(check = true);
    }
}

void Tank::rotateRight(int deg, bool check)
{
    degree += deg;
    if (degree > 360 || degree < -360)
        degree = 0;
    rotate();

    hdegree += deg;
    if (hdegree > 360 || hdegree < -360)
        hdegree = 0;
    hrotate();

    if (check != true)
    {
        while (isCollide() == true)
            rotateLeft(deg, check = true);
    }
}

void Tank::rotateLeft(int deg, bool check)
{
    degree -= deg;
    if (degree > 360 || degree < -360)
        degree = 0;
    rotate();

    hdegree -= deg;
    if (hdegree > 360 || hdegree < -360)
        hdegree = 0;
    hrotate();

    if (check != true)
    {
        while (isCollide() == true)
            rotateRight(deg, check = true);
    }
}

void Tank::headRight(bool check)
{
    if (hdegree + hspeed < 360)
        hdegree += hspeed;
    else
        hdegree = 0;
    hrotate();

    if (check != true)
    {
        while (isCollide() == true)
            headLeft(check = true);
    }
}

void Tank::headLeft(bool check)
{
    if (hdegree - hspeed > -360)
        hdegree -= hspeed;
    else
        hdegree = 0;
    hrotate();

    if (check != true)
    {
        while (isCollide() == true)
            headRight(check = true);
    }
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

void Tank::randomSpawn()
{
    int x,y;
    if (game->spawns > 0)
    {
        int random = rand()%game->spawns;
        x = game->spawnPoints[random]->x() - pixsize/2;
        y = game->spawnPoints[random]->y() - pixsize/4;
    }
    else
    {
        x = 200;
        y = 200;
    }

    setPos(x,y);
    head->setPos(x,y);

    degree = 0;
    hdegree = 0;
    rotateLeft(0);
}

void Tank::changeSize(int n)
{
    pixsize = n;
    setPixmap(QPixmap(baseImage).scaled(pixsize,pixsize));
    rotate();
    hrotate();
}

bool Tank::isCollide()
{
    QList<QGraphicsItem *> colliding_items = collidingItems();
    for (int i = 0, n = colliding_items.size(); i < n; ++i)
        if (typeid(*(colliding_items[i])) == typeid(Block))
            return true;

    QList<QGraphicsItem *> colliding_items1 = head->collidingItems();
    for (int j = 0, n = colliding_items1.size(); j < n; ++j)
        if (typeid(*(colliding_items1[j])) == typeid(Block))
            return true;

    return false;
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

























