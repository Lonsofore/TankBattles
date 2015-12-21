#ifndef BOT_H
#define BOT_H

#include "tank.h"
#include <QTimer>
#include <QTime>

class Bot: public Tank
{
    Q_OBJECT
public:
    Bot();
    void getDmg(int v);

private:
    int mode;
    QTimer *timer;

    QString line;
    QStringList list;

    bool out;
    QTime outTime;
    int outMaxTime;
    int outTimer;

public slots:
    void actions();
    void killBot();
    void spawnBot();
};

#endif // BOT_H
