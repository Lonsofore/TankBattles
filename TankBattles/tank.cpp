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

bool f = false;
int fireCount = 0;

Tank::Tank()
{
    int x = 300;
    int y = 200;

    // то, из-за чего программа жрет как майнкрафт
    keyDelay = 20;
    boost = 40; // ускорение танка в начале движения

    // стрельба
    fireReady = true;
    fireTime = 1000;
    bulletSpeed = 50;

    // обнуление
    xfix = 0;
    yfix = 0;
    mf = mb = rr = rl = hr = hl = fr = false;

    // платформа
    setPos(x,y);
    degree = 0;
    speed = 2;
    rspeed = 2;
    baseImage = ":/images/images/greenBase.png";
    setPixmap(QPixmap(baseImage));

    // башня
    head = new QGraphicsPixmapItem();
    head->setPos(x,y);
    hdegree = 0;
    hspeed = 2;
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
    switch ((uint) event->key())
    {
        // Двигаться вперед
        case 16777235: // Up
            mf = true;
        break;

        // Двигаться назад
        case 16777237: // Down
            mb = true;
        break;

        // Поворот танка налево
        case 16777234: // Left
            rl = true;
        break;

        // Поворот танка направо
        case 16777236: // Right
            rr = true;
        break;

        // Поворот башни налево
        case 90:   // Z
        case 1071: // Я
            hl = true;
        break;

        // Поворот башни направо
        case 88:   // X
        case 1063: // Ч
            hr = true;
        break;

        // Выстрел
        case 32: // SPAAAAACE
            fr = true;
        break;

        case 67:
            delay(2000);
            qDebug() << "test";
        break;
    }
    if (f == false)
    {
        qDebug() << "yay!";
        onKey();
    }
    //game->health->setPos(game->player->x()+40,game->player->y()+50);
}

void Tank::keyReleaseEvent(QKeyEvent *event) // то же самое, только отмена
{
    switch ((uint) event->key())
    {
        case 16777235: // Up
            mf = false;
        break;

        case 16777237: // Down
            mb = false;
        break;

        case 16777234: // Left
            rl = false;
        break;

        case 16777236: // Right
            rr = false;
        break;

        case 90:   // Z
        case 1071: // Я
            hl = false;
        break;

        case 88:   // X
        case 1063: // Ч
            hr = false;
        break;

        case 32: // SPAAAAACE
            fr = false;
        break;
    }
}

void Tank::onKey()
{
    f = true;
    int acc = boost;
    while (mf == true || mb == true || rl == true || rr == true || hl == true || hr == true || fr == true || fireReady == false)
    {
        if (mf) moveForward();
        if (mb) moveBack();
        if (rl) rotateLeft();
        if (rr) rotateRight();
        if (hl) headLeft();
        if (hr) headRight();

        if (fr && fireReady) fire();
        if (fireReady == false)
        {
            fireCount += keyDelay;
        }
        if (fireCount > fireTime)
        {
            fireReady = true;
            fireCount = 0;
        }

        if (acc > 0)
            acc--;

        delay(keyDelay + acc);
    }
    f = false;
    qDebug() << "end";
}

void Tank::moveForward()
{
    int x1,y1;
    int xadd = 0;
    int yadd = 0;
    double sn, cs;
    QPixmap tank(baseImage);

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

    // новые координаты
    x1 = x() + round(cs) - xadd;
    y1 = y() + round(sn) - yadd;
    if (y1 > 0 && y1 < scene()->height()-tank.size().height() && x1 > 0 && x1 < scene()->width()-tank.size().width())
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
    QPixmap tank(baseImage);

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
}

void Tank::rotateRight()
{
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
}

void Tank::rotateLeft()
{
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
    fireReady = false;

    int x1,y1;
    QPixmap tank(baseImage);

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

void Tank::delay( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}



























