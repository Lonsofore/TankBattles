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

bool action = false;
bool reload = false;
int fireCount = 0;

Tank::Tank()
{
    int x = 300;
    int y = 200;

    // то, из-за чего программа жрет как майнкрафт
    keyDelay = 20; // задержка между действиями клавиш
    boost = 40; // ускорение танка в начале движения
    iboost = 2; // по сколько отнимать от ускорения

    // стрельба
    fireReady = true;
    fireTime = 2000;
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
    baseImage = ":/images/images/tanks/greenBase.png";
    setPixmap(QPixmap(baseImage));

    // башня
    head = new QGraphicsPixmapItem();
    head->setPos(x,y);
    hdegree = 0;
    hspeed = 2;
    headImage = ":/images/images/tanks/greenHead.png";
    head->setPixmap(QPixmap(headImage));

    // звук выстрела
    bulletsound = new QMediaPlayer();
    bulletsound->setMedia(QUrl("qrc:/sounds/sounds/tank_fire.mp3"));

    //звук перезарядки
    bulletready = new QMediaPlayer();
    bulletready->setMedia(QUrl("qrc:/sounds/sounds/tank_reload.mp3"));

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
        case 87: // W
        case 1062: // Ц
        case 16777235: // Up
            mf = true;
        break;

        // Двигаться назад
        case 83: // S
        case 1067: // Ы
        case 16777237: // Down
            mb = true;
        break;

        // Поворот танка налево
        case 65: // A
        case 1060: // Ф
        case 16777234: // Left
            rl = true;
        break;

        // Поворот танка направо
        case 68: // D
        case 1042: // В
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
            setRotation(180);
            qDebug() << "test";
        break;
    }
    if (action == false)
    {
        //qDebug() << "start";
        if (mf || mb) // если танк двигается вперед или назад, то сделать ему замедление при начале движения
            onKey(boost);
        else
            onKey(0);
    }
    //game->health->setPos(game->player->x()+40,game->player->y()+50);
}

void Tank::keyReleaseEvent(QKeyEvent *event) // то же самое, только отмена
{
    switch ((uint) event->key())
    {
        case 87: // W
        case 1062: // Ц
        case 16777235: // Up
            mf = false;
        break;

        case 83: // S
        case 1067: // Ы
        case 16777237: // Down
            mb = false;
        break;

        case 65: // A
        case 1060: // Ф
        case 16777234: // Left
            rl = false;
        break;

        case 68: // D
        case 1042: // В
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

void Tank::onKey(int acc)
{
    action = true;
    //int acc = boost;
    while (mf == true || mb == true || rl == true || rr == true || hl == true || hr == true || fr == true || fireReady == false)
    {
        if (mf) moveForward();
        if (mb) moveBack();
        if (rl) rotateLeft();
        if (rr) rotateRight();
        if (hl) headLeft();
        if (hr) headRight();

        if (fr && fireReady)
        {
            fire();
            reload = false; // перезарядка еще на начата
        }
        if (fireReady == false) // счетчик для ожидания между выстрелами
        {
            fireCount += keyDelay;
        }
        if ((fireCount > fireTime - bulletready->duration()) && reload == false) // звук перезарядки
        {
            reload = true; // перезарядка уже начата (защита от нескольких)
            bulletready->play();
        }
        if (fireCount > fireTime) // готовность выстрела
        {
            fireReady = true;
            fireCount = 0;
        }

        if (acc > 0)
            acc = acc - iboost; // создается замедление в начале движения

        delay(keyDelay + acc);
    }
    action = false;
    //qDebug() << "stop";
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
    int deg = rspeed;
    if (mb) //если едет назад
        deg = -deg; // значит инверсия

    degree += deg;
    if (degree > 360 || degree < -360)
        degree = 0;
    rotate();

    hdegree += deg;
    if (hdegree > 360 || hdegree < -360)
        hdegree = 0;
    hrotate();

    //qDebug() << degree;
}

void Tank::rotateLeft()
{
    int deg = rspeed;
    if (mb) //если едет назад
        deg = -deg; // значит инверсия

    degree -= deg;
    if (degree > 360 || degree < -360)
        degree = 0;
    rotate();

    hdegree -= deg;
    if (hdegree > 360 || hdegree < -360)
        hdegree = 0;
    hrotate();

    //qDebug() << degree;
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



























