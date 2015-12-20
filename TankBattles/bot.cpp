#include "bot.h"
#include "game.h"

extern Game * game;

Bot::Bot()
{
    mode = 1;

    out = false;
    outMaxTime = 5; // секунд

    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(actions()));

    isPlayer = false;
    isBot = true;
    defaultTank();

    timer->start(20);
}

void Bot::actions()
{
    QString line;
    QStringList list;

    line = bot->readLine();
    list = line.split(" ");

    for (int i = 0; i < list.count(); i++)
    {
        // если player вышел за границы
        if (this->x() < game->dop - pixsize/2 ||
            this->x() > game->scene->width()-game->dop*2 + 20 ||
            this->y() < game->dop - pixsize/2 ||
            this->y() > game->scene->height()-game->dop*2 + 20)
        {
            if (out == false)
            {
                out = true;
                outTime = QTime::currentTime();
                outTimer = outMaxTime;
            }

            if (outTime.msecsTo(QTime::currentTime()) > 1000)
            {
                if (outTimer > 0)
                {
                    outTimer--;
                    outTime = outTime.addSecs(1);
                }
                else
                {
                    // DIE MOTHERFUCKER!
                    out = false;
                    outTimer = outMaxTime;
                    killBot();
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
            }
        }

        if (list[i] == "mf")
            moveForward();
        if (list[i] == "mb")
            moveBack();
        if (list[i] == "rr")
            rotateRight();
        if (list[i] == "rl")
            rotateLeft();
        if (list[i] == "hr")
            headRight();
        if (list[i] == "hl")
            headLeft();
        if (list[i] == "fr")
            fire();
    }

    if (bot->atEnd())
    {
        switch (mode)
        {
            case 0:
                timer->stop();
                killBot();
            break;

            case 1:
                bot->reset();
            break;
        }
    }

}

void Bot::killBot()
{
    deleteTank();
    QTimer::singleShot(2000, this, SLOT(spawnBot()));
}

void Bot::spawnBot()
{
    spawnTank();
    timer->start(20);
}

