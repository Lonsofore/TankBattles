#include "button.h"
#include <QGraphicsPixmapItem>
#include <QTime>
#include <QCoreApplication>

bool pressed = false;

Button::Button(QString n, int x, int y, QGraphicsItem *parent) : QGraphicsPixmapItem(parent)
{
    name = n;
    width = x;
    height = y;

    QString image = ":/images/images/menu/" + name + ".png";
    setPixmap(QPixmap(image).scaled(width,height));

    setAcceptHoverEvents(true);
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pressed = true;
    QString image = ":/images/images/menu/" + name + "Chose.png";
    setPixmap(QPixmap(image).scaled(width,height));

    delay(1000);
    emit clicked();
}

void Button::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (pressed == false)
    {
        QString image = ":/images/images/menu/" + name + "Select.png";
        setPixmap(QPixmap(image).scaled(width,height));
    }
}

void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if (pressed == false)
    {
        QString image = ":/images/images/menu/" + name + ".png";
        setPixmap(QPixmap(image).scaled(width,height));
    }
}

void Button::delay( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}






















