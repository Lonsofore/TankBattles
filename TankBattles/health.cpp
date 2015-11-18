#include "Health.h"
#include <QFont>

Health::Health(QGraphicsItem *parent) : QGraphicsTextItem(parent)
{
    health = 100;

    setPlainText(QString::number(health));
    setDefaultTextColor(Qt::black);
    setFont(QFont("times",14));
}

void Health::decrease(int v)
{
    if (health - v > 0)
        health -= v;
    else
        health = 0;
    setPlainText(QString::number(health));
}

int Health::getHealth()
{
    return health;
}


