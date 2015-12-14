#ifndef PLAYER
#define PLAYER

#include "tank.h"
#include "textpanel.h"
#include "caution.h"
#include "button.h"
#include <QPixmap>

class Player: public Tank
{
    Q_OBJECT
public:
    Player();
    // вызов действий для кнопок
    int keyDelay; // задержка между проверками кнопок
    void keyPressEvent(QKeyEvent *event);   // считывание нажатий
    void keyReleaseEvent(QKeyEvent *event); // и отжатий
    void onKey(int acc); // если кнопка нажата - тикер

    int boost;  // ускорение в начале движения
    int iboost; // по сколько отнимать от ускорения
    int fireTime;    // время между выстрелами

    int centralX();
    int centralY();

    void playerRotate();
    void playerFire();
    void killPlayer();

    void playerReset(); // сбросить значения движений

    double GetX() const;
    double GetY() const;
    int GetTAngle() const;
    int GetHAngle() const;

private:
    // соответственно с объявленными функциями для действий
    bool mf, mb, rr, rl, hr, hl, fr;

    Caution *caution;
    Caution *died;

public slots:
    void spawnPlayer();

signals:
    void tomenu();
    void KeyPressed();
};

#endif // PLAYER

