#include "player.h"
#include "game.h"
#include <qglobal.h>
#include <QTimer>
#include <QLabel>
#include <QMovie>
#include <QTime>

#include <QDebug>

bool action = false;

bool reloading = false;
bool fireReady = true;
int fireCount = 0;

bool out = false;
QTime outTime;
int outMaxTime = 5; // секунд
int outTimer;

int spin = 0;

int nMenuBtns = 4;
int curBtn;

bool alive;

int lastX, lastY, lastDeg, lastHDeg, lastHealth;

bool wou;

QTime last;

extern Game * game;

Player::Player()
{
    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(keyActions()));

    keyDelay = 20; // задержка между действиями клавиш

    wou = false;

    // нажатые кнопки
    mf = mb = rr = rl = hr = hl = fr = false;

    // стрельба
    fireTime = 2000;

    alive = true;
    isFiring = 0;

    defaultTank();
    emit reSpawn();

    game->centerOn(this);

    // выделить танк на сцене - для действий с ним
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();
}

void Player::keyPressEvent(QKeyEvent *event)
{
    //qDebug() << (uint) event->key(); // чтобы узнать код клавиши
    //qDebug() << event->text();
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
                /*
                QLabel *gif_anim = new QLabel();
                gif_anim->setStyleSheet("background-color: rgba(229, 229, 229, 10);");
                QMovie *movie = new QMovie(":/images/images/anim/Explo.gif");
                gif_anim->setMovie(movie);
                gif_anim->move(x()-25,y()-25);
                movie->setScaledSize(QSize(250,250));
                movie->start();
                QGraphicsProxyWidget *proxy = game->scene->addWidget(gif_anim);
                */
            break;

            case 53:
                //getDmg(10);
                last = QTime::currentTime();
            break;

            case 54:
                //qDebug() << cur.minute() << " " << cur.second() << " " << cur.msec();
                qDebug() << last.msecsTo(QTime::currentTime());
            break;
        }
        if (action == false)
        {
            if (alive)
            {
                action = true;
                onKey();
            }
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

void Player::onKey()
{
    keyActions();
    emit KeyPressed();

    if (alive == true && (mf == true || mb == true || rl == true || rr == true || hl == true || hr == true || fr == true ||
                          fireReady == false || tankhrotate->state() == QMediaPlayer::PlayingState || out == true))
    {
        timer->start(keyDelay);
    }
}

void Player::keyActions() // player действия при нажатии клавиши
{
    game->centerOn(this);

    lastX = x();
    lastY = y();
    lastDeg = degree;
    lastHDeg = hdegree;
    lastHealth = health;

    // эффект получения урона (должен стоять перед движением)
    if (wou)
    {
        /*
        int x1 = centralX();
        int y1 = centralY();
        wound->setPos(x1-game->width()/2,y1-game->height()/2);
        */
    }

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
            outTime = QTime::currentTime();
            outTimer = outMaxTime;

            QString image = ":/images/images/out.png";
            caution = new Caution(image, "CAUTION", 5);
            caution->setPos(x1-200,y1-88);
            caution->setZValue(10);
            game->scene->addItem(caution);
        }
        caution->setPos(x1-200, y1-88);

        if (QTime::currentTime().msecsTo(outTime) > 1000)
        {
            if (outTimer > 0)
            {
                outTimer--;
                caution->decTimer();
            }
            else
            {
                // DIE MOTHERFUCKER!
                //qDebug() << "you died";
                out = false;
                outTimer = outMaxTime;
                delete caution;

                // плашка смерти
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
            outTimer = outMaxTime;
            delete caution;
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
        reloading = false; // звук перезарядки еще на начат
        isFiring = 1;

        reload = 1;
        // анимация перезарядки
        reloadAnim = new QLabel();
        reloadAnim->setStyleSheet("background-color: transparent;");
        QMovie *movie = new QMovie(":/images/images/anim/Reload.gif");
        reloadAnim->setMovie(movie);
        reloadAnim->move(x()+20,y()+20);
        movie->setScaledSize(QSize(180,180));
        movie->setSpeed(110);
        movie->start();
        QGraphicsProxyWidget *proxy = game->scene->addWidget(reloadAnim);

    }
    if (fireReady == false) // счетчик для ожидания между выстрелами
    {
        fireCount += keyDelay;

        reloadAnim->move(x()+20,y()+20);
    }
    if ((fireCount > fireTime - bulletready->duration()) && reload == false) // звук перезарядки
    {
        reloading = true; // перезарядка уже начата (защита от нескольких)
        bulletready->play();
    }
    if (fireCount > fireTime) // готовность выстрела
    {
        fireReady = true;
        fireCount = 0;

        reload = 0;
        delete reloadAnim;
    }

    if (lastX != x() || lastY != y() || lastDeg != degree || lastHDeg != hdegree || lastHealth != health)
        emit KeyPressed();

    if (alive == false || (mf == false && mb == false && rl == false && rr == false && hl == false && hr == false && fr == false &&
                          fireReady == true && tankhrotate->state() == QMediaPlayer::StoppedState && out == false))
    {
        action = false;
        timer->stop();
    }
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
    emit reSpawn();
    game->centerOn(this);
    setFocus();

}

void Player::getDmg(int v)
{
    decHealth(v);
    wou = true;

    int x1 = centralX();
    int y1 = centralY();

    wound = new QGraphicsPixmapItem();
    wound->setPixmap(QPixmap(":/images/images/wound.png").scaled(game->width(),game->height()));
    wound->setPos(x1-game->width()/2,y1-game->height()/2);
    game->scene->addItem(wound);

    /*
    QTimer *timerw = new QTimer();
    connect(timerw,SIGNAL(timeout()),this,SLOT(playerWound()));
    */
    QTimer::singleShot(2000, this, SLOT(playerWound()));
}

void Player::playerWound()
{
    if (wou)
    {
        delete wound;
        wou = false;
    }
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
