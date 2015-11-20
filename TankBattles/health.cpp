#include "health.h"
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

void Health::set(int v)
{
    setPlainText(QString::number(v));
}

int Health::getHealth()
{
    return health;
}


