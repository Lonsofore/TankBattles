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
    //bool fireReady; // можно ли уже стрелять
    int fireTime;   // время между выстрелами

    int boost;  // ускорение в начале движения
    int iboost; // по сколько отнимать от ускорения

public slots:
    void spawn();

private:
    // соответственно с объявленными функциями для действий

};

#endif // PLAYER

