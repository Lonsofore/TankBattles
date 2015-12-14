#include "player.h"
#include "game.h"
#include "delay.h"
#include <qglobal.h>
#include <QTimer>
#include <QLabel>
#include <QMovie>

#include <QDebug>

bool action = false;
bool reload = false;
bool fireReady = true;
int fireCount = 0;

bool out = false;
int outCount = 0;
int outTimer = 5;

int spin = 0;

int nMenuBtns = 4;
int curBtn;

bool alive;

extern Game * game;

Player::Player()
{
    keyDelay = 20; // задержка между действиями клавиш

    // то, из-за чего программа жрет как майнкрафт
    boost = 40; // ускорение танка в начале движения
    iboost = 2; // по сколько отнимать от ускорения
    mf = mb = rr = rl = hr = hl = fr = false;

    // стрельба
    fireTime = 2000;

    alive = true;

    defaultTank();
    game->centerOn(this);

    // выделить танк на сцене - для действий с ним
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();
}

void Player::keyPressEvent(QKeyEvent *event)
{
    //qDebug() << (uint) event->key(); // чтобы узнать код клавиши
    if (alive)
    {
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
            case 81:   // Q
            case 1049: // Й
            case 90:   // Z
            case 1071: // Я
                hl = true;
            break;

            // Поворот башни направо
            case 69:   // E
            case 1059: // У
            case 88:   // X
            case 1063: // Ч
                hr = true;
            break;

            // Выстрел
            case 32: // SPAAAAACE
                fr = true;
            break;

            // Меню
            case 16777216: // Esc
                //ingame = false;
                emit tomenu();
            break;

            // Таблица
            case 96:   // `
            case 1025: // ё
                qDebug() << "score";
            break;

            // цифры (для тестов)
            case 49:
                this->changeSize(pixsize-10);
                qDebug() << pixsize;
            break;

            case 50:
                this->changeSize(pixsize+10);
                qDebug() << pixsize;
            break;

            case 51:
                randomSpawn();
            break;

            case 52:
                QLabel *gif_anim = new QLabel();
                gif_anim->setStyleSheet("background-color: rgba(229, 229, 229, 10);");
                QMovie *movie = new QMovie(":/images/images/anim/Explo.gif");
                gif_anim->setMovie(movie);
                gif_anim->move(x()-25,y()-25);
                movie->setScaledSize(QSize(250,250));
                movie->start();
                QGraphicsProxyWidget *proxy = game->scene->addWidget(gif_anim);

            break;
        }
        if (action == false)
        {
            //qDebug() << "start";
            if (mf || mb) // если танк движется вперед или назад, то сделать ему замедление при начале движения
                onKey(boost);
            else
                onKey(0);
        }
        //game->health->setPos(game->player->x()+40,game->player->y()+50);
    }
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

        case 81:   // Q
        case 1049: // Й
        case 90:   // Z
        case 1071: // Я
            hl = false;
        break;

        case 69:   // E
        case 1059: // У
        case 88:   // X
        case 1063: // Ч
            hr = false;
        break;

        case 32: // SPAAAAACE
            fr = false;
        break;
    }
}

void Player::onKey(int acc) // player действия при нажатии клавиши. acc - задержка танка при старте
{
    action = true;
    while (alive == true && (mf == true || mb == true || rl == true || rr == true || hl == true || hr == true || fr == true ||
           fireReady == false || tankhrotate->state() == QMediaPlayer::PlayingState || out == true))
    {
        game->centerOn(this);

        // движение вперед, назад
        if (mf) moveForward();
        if (mb) moveBack();

        // если player вышел за границы
        if (this->x() < game->dop - pixsize/2 ||
            this->x() > game->scene->width()-game->dop*2 + 20 ||
            this->y() < game->dop - pixsize/2 ||
            this->y() > game->scene->height()-game->dop*2 + 20)
        {
            // координата Х в центре экрана
            int x1 = centralX();

            // координата Y в центре экрана
            int y1 = centralY();

            if (out == false)
            {
                out = true;

                QString image = ":/images/images/out.png";
                caution = new Caution(image, "CAUTION", 5);
                caution->setPos(x1-200,y1-88);
                caution->setZValue(10);
                game->scene->addItem(caution);
            }
            caution->setPos(x1-200, y1-88);


            outCount += keyDelay;
            if (outCount > 1000)
            {
                outCount = 0;
                if (outTimer > 0)
                {
                    //qDebug() << outTimer;
                    outTimer--;
                    caution->decTimer();
                }
                else
                {
                    // DIE MOTHERFUCKER!
                    //qDebug() << "you died";
                    out = false;
                    outTimer = 5;
                    delete caution;

                    QString image1 = ":/images/images/died.png";
                    died = new Caution(image1, "YOU DIED");
                    died->setPos(x1-200,y1-88);
                    died->setZValue(10);
                    game->scene->addItem(died);

                    // убить
                    killPlayer();
                    // BUHAHA
                }
            }
        }
        else
        {
            if (out == true)
            {
                out = false;
                outTimer = 5;
                delete caution;
                //delete noise;
            }
        }

        // универсальный поворот
        if (rl || rr)
        {
            playerRotate();
            spin++;
        }

        // ты ничего не видел
        if (rl == false && rr == false)
            spin = 0;

        // совершенно ничего
        if (spin == 540)
        {
            QMediaPlayer *sound = new QMediaPlayer();
            sound->setMedia(QUrl("qrc:/sounds/sounds/spin.mp3"));
            sound->play();
        }

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

        emit KeyPressed();
        delay(keyDelay + acc);
    }
    action = false;
}

// координата X в центре экрана
int Player::centralX()
{
    int x1;
    if (this->x() + pixsize/2 < game->width()/2)
        x1 = game->width()/2;
    if (this->x() + pixsize/2 > game->scene->width() - game->width()/2)
        x1 = game->scene->width() - game->width()/2;
    if (this->x() + pixsize/2 > game->width()/2 && this->x() + pixsize/2 < game->scene->width() - game->width()/2)
        x1 = this->x() + pixsize/2;
    return x1;
}

// координата Y в центре экрана
int Player::centralY()
{
    int y1;
    if (this->y() + pixsize/2 < game->height()/2)
        y1 = game->height()/2;
    if (this->y() + pixsize/2 > game->scene->height() - game->height()/2)
        y1 = game->scene->height() - game->height()/2;
    if (this->y() + pixsize/2 > game->height()/2 && this->y() + pixsize/2 < game->scene->height() - game->height()/2)
        y1 = this->y() + pixsize/2;
    return y1;
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

void Player::killPlayer()
{
    deleteTank();
    QTimer::singleShot(2000, this, SLOT(spawnPlayer()));
}

void Player::spawnPlayer()
{
    //qDebug() << "spawn!";
    delete died;
    spawnTank();
    game->centerOn(this);
    setFocus();
}

void Player::playerReset()
{
    mf = mb = rr = rl = hr = hl = fr = false;
}

double Player::GetX() const
{
    return qRound(this->x());
}

double Player::GetY() const
{
    return qRound(this->y());
}

int Player::GetTAngle() const
{
    return this->degree;
}

int Player::GetHAngle() const
{
    return this->hdegree;
}
