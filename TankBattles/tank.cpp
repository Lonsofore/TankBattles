#include "tank.h"
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "bullet.h"
#include "game.h"
#include <QTime>
#include <QCoreApplication>
#include <typeinfo>
#include <cstdlib>
#include <QTimer>
#include <QMovie>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <math.h>
#define PI 3.14159265

#include <QDebug>
int toInt(double x);
extern Game * game;

Tank::Tank()
{
    // отдельная функция для того, чтобы ее вызывать классом player
    defaultTank();
}

void Tank::defaultTank()
{
    // размер танка
    pixsize = 220;

    // обнуление
    xfix = 0;
    yfix = 0;

    alive = true;
    maxhealth = 100;
    health = maxhealth;

    // платформа
    degree = 0;
    speed = 2;
    rspeed = 2;
    baseImage = ":/images/images/tanks/greenBase.png";
    setPixmap(QPixmap(baseImage).scaled(pixsize,pixsize));
    //setPos(x,y);
    setZValue(80); // "высота" платформы

    // башня
    head = new QGraphicsPixmapItem();
    hdegree = 0;
    hspeed = 2;
    headImage = ":/images/images/tanks/greenHead.png";
    head->setPixmap(QPixmap(headImage).scaled(pixsize,pixsize));
    //head->setPos(x,y);
    head->setZValue(90);

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
    tankhrotate = new QMediaPlayer();
    tankhrotate->setMedia(QUrl("qrc:/sounds/sounds/tank_hrotate.wav"));
    tankhrotate->setVolume(game->veffects);
}



void Tank::moveForward(bool check)
{
    float x1,y1;

    // новые координаты
    x1 = x() + cos(degree * (PI / 180))*speed;
    y1 = y() + sin(degree * (PI / 180))*speed;

    setPos(x1,y1);
    head->setPos(x1,y1);

    if (check == false)
    {
        while (isCollide() == true)
            moveBack(check = true);
    }
}

void Tank::moveBack(bool check)
{
    float x1,y1;

    // новые координаты
    x1 = x() - cos(degree * (PI / 180))*speed;
    y1 = y() - sin(degree * (PI / 180))*speed;

    setPos(x1,y1);
    head->setPos(x1,y1);

    if (check == false)
    {
        while (isCollide() == true)
            moveForward(check = true);
    }
}

// проверка градуса поворота
int Tank::checkDegree(int deg)
{
    if (deg > 360)
        deg -= 360;
    if (deg < 0)
        deg += 360;
    return deg;
}

void Tank::rotateRight(bool check)
{
    int deg = rspeed;
    degree = checkDegree(degree + deg);
    rotate();

    hdegree = checkDegree(hdegree + deg);
    hrotate();

    if (check == false)
    {
        while (isCollide() == true)
            rotateLeft(check = true);
    }
}

void Tank::rotateLeft(bool check)
{
    int deg = rspeed;
    degree = checkDegree(degree - deg);
    rotate();

    hdegree = checkDegree(hdegree - deg);
    hrotate();

    if (check == false)
    {
        while (isCollide() == true)
            rotateRight(check = true);
    }
}

void Tank::rotateRight(int deg, bool check)
{
    degree = checkDegree(degree + deg);
    rotate();

    hdegree = checkDegree(hdegree + deg);
    hrotate();

    if (check == false)
    {
        while (isCollide() == true)
            rotateLeft(deg, check = true);
    }
}

void Tank::rotateLeft(int deg, bool check)
{
    degree = checkDegree(degree - deg);
    rotate();

    hdegree = checkDegree(hdegree - deg);
    hrotate();

    if (check == false)
    {
        while (isCollide() == true)
            rotateRight(deg, check = true);
    }
}

void Tank::headRight(bool check)
{
    hdegree = checkDegree(hdegree + hspeed);
    hrotate();

    if (check == false)
    {
        while (isCollide() == true)
            headLeft(check = true);
    }
}

void Tank::headLeft(bool check)
{
    hdegree = checkDegree(hdegree - hspeed);
    hrotate();

    if (check == false)
    {
        while (isCollide() == true)
            headRight(check = true);
    }
}

void Tank::headRight(int deg, bool check)
{
    hdegree = checkDegree(hdegree + deg);
    hrotate();

    if (check == false)
    {
        while (isCollide() == true)
            headLeft(check = true);
    }
}

void Tank::headLeft(int deg, bool check)
{
    hdegree = checkDegree(hdegree - deg);
    hrotate();

    if (check == false)
    {
        while (isCollide() == true)
            headRight(check = true);
    }
}

void Tank::fire()
{
    int x1,y1;
    int bulsize = pixsize*0.16;

    // левая верхняя позиция танка + половина размера танка (чтобы из середины)
    x1 = x() + pixsize/2 + toInt(cos(hdegree * PI / 180) * (pixsize/2 + pixsize/20)) - bulsize/2;
    y1 = y() + pixsize/2 + toInt(sin(hdegree * PI / 180) * (pixsize/2 + pixsize/20)) - bulsize/2;

    Bullet *bullet = new Bullet(this);
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
        int stime;
        long ltime;
        int random;

        ltime = time(NULL);
        stime = (unsigned) ltime/2;
        srand(stime);

        random = rand()%game->spawns;
        x = game->spawnPoints[random]->x() - pixsize/2;
        y = game->spawnPoints[random]->y() - pixsize/4;

        while (isBusy(x,y))
        {
            random++;
            if (random > game->spawns-1)
                random = 0;

            x = game->spawnPoints[random]->x() - pixsize/2;
            y = game->spawnPoints[random]->y() - pixsize/4;
        }
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

// true если на координатах x,y уже находится танк
bool Tank::isBusy(int x, int y)
{
    // все коэффициенты брались по платформе относительно общей картинки. башня во внимание не бралась (она не соприкасается)
    QList<QGraphicsItem *> items = game->scene->items(x+0.28*pixsize,y+0.32*pixsize,pixsize*0.44,pixsize*0.36,Qt::IntersectsItemShape,Qt::AscendingOrder,QTransform());
    if (items.count() > 1)
        return true;
    return false;
}

void Tank::changeSize(int n)
{
    pixsize = n;
    setPixmap(QPixmap(baseImage).scaled(pixsize,pixsize));
    rotate();
    hrotate();
}

// возвращает true, если танк с чем-то соприкасается и пускать туда нельзя
// возвращает false, если танк не соприкасается ни с чем и все ок
bool Tank::isCollide()
{
    QList<QGraphicsItem *> colliding_items = collidingItems();    
    for (int i = 0, n = colliding_items.size(); i < n; ++i)
    {
        if (typeid(*(colliding_items[i])) == typeid(Tank))
        {
                return true;
        }

        if (typeid(*(colliding_items[i])) == typeid(Block))
        {
            Block * block = dynamic_cast <Block *> (colliding_items[i]);
            if (block->num == 3) // трава
                return false;
            else
                return true;
        }
    }

    QList<QGraphicsItem *> colliding_items1 = head->collidingItems();
    for (int j = 0, n = colliding_items1.size(); j < n; ++j)
    {
        if (typeid(*(colliding_items1[j])) == typeid(Block))
        {
            Block * block1 = dynamic_cast <Block *> (colliding_items1[j]);
            if (block1->num == 3) // трава
                return false;
            else
                return true;
        }
    }

    return false;
}

// с возвратом qpixmap какой-то гемор, потому функций сделано две
void Tank::rotate() // поворот платформы
{
    if (alive == true)
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
}

void Tank::hrotate() // поворот башни
{
    if (alive == true)
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
}

void Tank::decHealth(int v)
{
    health -= v;
    if (health < 0)
        killTank();
}

void Tank::deleteSlot()
{
    deleteTank();
}

void Tank::deleteTank()
{
    alive = false;
    head->setPixmap(QPixmap(":/images/images/empty.png").scaled(pixsize,pixsize));
    setPixmap(QPixmap(":/images/images/empty.png").scaled(pixsize,pixsize));

    // анимация
    gif_anim = new QLabel();
    gif_anim->setStyleSheet("background-color: rgba(229, 229, 229, 10);");
    QMovie *movie = new QMovie(":/images/images/anim/Explo.gif");
    gif_anim->setMovie(movie);
    gif_anim->move(x()-25,y()-25);
    movie->setScaledSize(QSize(250,250));
    movie->start();
    QGraphicsProxyWidget *proxy = game->scene->addWidget(gif_anim);
    QTimer::singleShot(2500, this, SLOT(deleteGif()));
}

void Tank::deleteGif()
{
    delete gif_anim;
}

void Tank::spawnSlot()
{
    spawnTank();
}

void Tank::spawnTank()
{
    alive = true;
    health = maxhealth;
    setPixmap(QPixmap(baseImage).scaled(pixsize,pixsize));
    head->setPixmap(QPixmap(headImage).scaled(pixsize,pixsize));
    randomSpawn();
}

// для ботов

void Tank::killTank()
{
    deleteTank();
    QTimer::singleShot(2000, this, SLOT(spawnSlot()));
}

// для мультиплеера

void Tank::changePos(int x, int y)
{
    setPos(x,y);
    head->setPos(x,y);
}

void Tank::changeAngle(int TAngle, int HAngle)
{
    this->degree = TAngle;
    this->rotateLeft(0,true);
    this->hdegree = HAngle;
    this->rotateLeft(0,true);
}

int toInt(double x)
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
