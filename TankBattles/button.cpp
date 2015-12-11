#include "button.h"
#include <QGraphicsPixmapItem>
#include <QTime>
#include <QCoreApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QFontDatabase>
#include <QKeyEvent>
#include "delay.h"

bool pressed = false;

Button::Button(int n, QString name, int x, int y, QGraphicsItem *parent) : QGraphicsPixmapItem(parent)
{
    num = n;
    width = x;
    height = y;

    QString image = ":/images/images/menu/Shape.png";
    setPixmap(QPixmap(image).scaled(width,height));

    // добавление шрифта
    QFontDatabase::addApplicationFont(":/fonts/fonts/GOTHIC.TTF");

    // текст
    text = new QGraphicsTextItem(name,this);
    text->setDefaultTextColor(QColor(71, 71, 71, 255));
    QFont font("Century Gothic",38);
    text->setFont(font);
    int xPos = this->width/2 - text->boundingRect().width()/2;
    int yPos = this->height/2 - text->boundingRect().height()/2 - 5;
    text->setPos(xPos,yPos);

    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsFocusable); // кнопку можно выделить (для нажатий клавиш)
}

void Button::keyPressEvent(QKeyEvent *event)
{
    switch ((uint) event->key())
    {
        case 16777235: // Up
            emit changed(num-1);
        break;

        case 16777237: // Down
            emit changed(num+1);
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
    select();
    emit changed(num);
}

void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    deselect();
}

void Button::setText(QString name)
{
    text->setPlainText(name);
    int xPos = this->width/2 - text->boundingRect().width()/2;
    int yPos = this->height/2 - text->boundingRect().height()/2 - 5;
    text->setPos(xPos,yPos);
}

void Button::select()
{
    if (pressed == false)
    {
        QString image = ":/images/images/menu/Select.png";
        setPixmap(QPixmap(image).scaled(width,height));
        setFocus();
    }
}

void Button::deselect()
{
    if (pressed == false)
    {
        QString image = ":/images/images/menu/Shape.png";
        setPixmap(QPixmap(image).scaled(width,height));
    }
}

void Button::click()
{
    if (pressed == false)
    {
        pressed = true;

        QString image = ":/images/images/menu/Chose.png";
        setPixmap(QPixmap(image).scaled(width,height));

        delay(1000);
        pressed = false;
        emit clicked();
    }
}





















