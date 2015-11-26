#ifndef PLAYER
#define PLAYER

#include "tank.h"

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

    void playerRotate();
    void playerFire();

    void playerReset(); // сбросить значения движений
public slots:
    void spawn();

private:
    // соответственно с объявленными функциями для действий
    bool mf, mb, rr, rl, hr, hl, fr;


};

#endif // PLAYER

