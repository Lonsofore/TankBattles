#include "numupdown.h"
#include "game.h"
#include <QGraphicsPixmapItem>
#include <QTime>
#include <QCoreApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QFontDatabase>

extern Game * game;
const int anum = 11;

numUpDown::numUpDown(QString arr[], int n, int x, int y, QGraphicsItem *parent) : QGraphicsPixmapItem(parent)
{
    num = n;
    width = x;
    height = y;

    for (int i = 0; i < anum; i++)
    {
        array[i] = arr[i];
    }

    QString image = ":/images/images/menu/Shape.png";
    setPixmap(QPixmap(image).scaled(width,height));

    // шрифт
    QFontDatabase::addApplicationFont(":/fonts/fonts/GOTHIC.TTF");

    // текст
    text = new QGraphicsTextItem(array[num],this);
    text->setDefaultTextColor(QColor(71, 71, 71, 255));
    QFont font("Century Gothic",38);
    text->setFont(font);
    int xPos = this->width/2 - text->boundingRect().width()/2;
    int yPos = this->height/2 - text->boundingRect().height()/2 - 5;
    text->setPos(xPos,yPos);

    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsFocusable); // кнопку можно выделить (для нажатий клавиш)
}

void numUpDown::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    click();
}

void numUpDown::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    select();
    emit entered(num);
}

void numUpDown::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    deselect();
}

void numUpDown::setText()
{
    text->setPlainText(array[num]);

    int xPos = this->width/2 - text->boundingRect().width()/2;
    int yPos = this->height/2 - text->boundingRect().height()/2 - 5;
    text->setPos(xPos,yPos);

}

void numUpDown::select()
{
    QString image = ":/images/images/menu/Select.png";
    setPixmap(QPixmap(image).scaled(width,height));
    setFocus();
}

void numUpDown::deselect()
{
    QString image = ":/images/images/menu/Shape.png";
    setPixmap(QPixmap(image).scaled(width,height));
}

void numUpDown::click()
{
    //QString image = ":/images/images/menu/Chose.png";
    //setPixmap(QPixmap(image).scaled(width,height));

    if (num < anum-1)
        num++;
    else
        num = 0;

    setText();

    emit clicked();
}
