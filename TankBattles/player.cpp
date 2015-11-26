#include "player.h"
#include "enemy.h"
#include "game.h"
#include <QDebug>

bool action = false;
bool reload = false;
bool fireReady = true;
int fireCount = 0;

Player::Player()
{
    keyDelay = 20; // задержка между действиями клавиш
    int x = 300;
    int y = 200;

    // то, из-за чего программа жрет как майнкрафт
    boost = 40; // ускорение танка в начале движения
    iboost = 2; // по сколько отнимать от ускорения
    mf = mb = rr = rl = hr = hl = fr = false;

    // стрельба
    fireTime = 2000;
    bulletSpeed = 50;

    defaultTank(x,y);

    // выделить танк на сцене - для действий с ним
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();
}

void Player::keyPressEvent(QKeyEvent *event)
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
            qDebug() << "just yay";
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

void Player::keyReleaseEvent(QKeyEvent *event) // то же самое, только отмена
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

void Player::onKey(int acc)
{
    action = true;
    //int acc = boost;
    while (mf == true || mb == true || rl == true || rr == true || hl == true || hr == true || fr == true || fireReady == false || tankhrotate->state() == QMediaPlayer::PlayingState)
    {
        if (mf) moveForward();
        if (mb) moveBack();

        // универсальный поворот
        if (rl || rr) playerRotate();

        // поворот башни
        if (hl) headLeft();
        if (hr) headRight();

        // звук поворота башни
        if ((hr || hl) &&  tankhrotate->state() == QMediaPlayer::StoppedState) // если звук поворота еще не проигрывался
            tankhrotate->play();

        if (hr == false && hl == false && tankhrotate->state() == QMediaPlayer::PlayingState)
            tankhrotate->stop();

        // стрельба + звуки стрельбы
        if (fr && fireReady)
        {
            playerFire();
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
}

void Player::playerRotate()
{
    int deg = rspeed;
    if (mb) //если едет назад
        deg = -deg; // значит инверсия

    // выбор стороны поворота
    if (rl)
        rotateLeft(deg);
    if (rr)
        rotateRight(deg);
}

void Player::playerFire()
{
    fireReady = false;
    fire();
}

void Player::spawn()
{
    Enemy *enemy = new Enemy();
    scene()->addItem(enemy);
}
