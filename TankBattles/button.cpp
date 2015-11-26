#include "button.h"
#include "game.h"
#include <QGraphicsPixmapItem>
#include <QTime>
#include <QCoreApplication>
#include <QGraphicsScene>
#include <QGraphicsView>

extern Game * game;

Button::Button(int n, QString text, int x, int y, QGraphicsItem *parent) : QGraphicsPixmapItem(parent)
{
    num = n;
    name = text;
    width = x;
    height = y;

    QString image = ":/images/images/menu/" + name + ".png";
    setPixmap(QPixmap(image).scaled(width,height));

    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsFocusable); // кнопку можно выделить (для нажатий клавиш)
}

void Button::keyPressEvent(QKeyEvent *event)
{
    switch ((uint) event->key())
    {
        case 16777235: // Up
            if (num > 0)
                game->curButton--;
            else
                game->curButton = game->numsButtons-1;
            game->switchButton();
        break;

        case 16777237: // Down
            if (num < game->numsButtons-1)
                game->curButton++;
            else
                game->curButton = 0;
            game->switchButton();
        break;

        case 16777220: // Enter
            click();
        break;
    }
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    click();
}

void Button::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    game->curButton = num;
    game->switchButton();
}

void Button::select()
{
    if (game->pressed == false)
    {
        QString image = ":/images/images/menu/" + name + "Select.png";
        setPixmap(QPixmap(image).scaled(width,height));
        setFocus();
    }
}

void Button::deselect()
{
    if (game->pressed == false)
    {
        QString image = ":/images/images/menu/" + name + ".png";
        setPixmap(QPixmap(image).scaled(width,height));
    }
}

void Button::click()
{
    if (game->pressed == false)
    {
        game->pressed = true;

        QString image = ":/images/images/menu/" + name + "Chose.png";
        setPixmap(QPixmap(image).scaled(width,height));

        delay(1000);
        emit clicked();
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






















