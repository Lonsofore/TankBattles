#include "numupdown.h"
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QTime>
#include <QCoreApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QFontDatabase>

numUpDown::numUpDown(int n, QString arr[], int c, int def, int x, int y, QGraphicsItem *parent) : QGraphicsPixmapItem(parent)
{
    num = n;
    count = c;
    curr = def;
    width = x;
    height = y;

    for (int i = 0; i < count; i++)
    {
        array[i] = arr[i];
    }

    QString image = ":/images/images/menu/Shape.png";
    setPixmap(QPixmap(image).scaled(width,height));

    // шрифт
    QFontDatabase::addApplicationFont(":/fonts/fonts/GOTHIC.TTF");

    // текст
    text = new QGraphicsTextItem(array[curr],this);
    text->setDefaultTextColor(QColor(71, 71, 71, 255));
    QFont font("Century Gothic",38);
    text->setFont(font);
    int xPos = this->width/2 - text->boundingRect().width()/2;
    int yPos = this->height/2 - text->boundingRect().height()/2 - 5;
    text->setPos(xPos,yPos);

    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsFocusable); // кнопку можно выделить (для нажатий клавиш)
}

void numUpDown::keyPressEvent(QKeyEvent *event)
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

        case 16777216: // Esc
            emit back();
        break;
    }
}

void numUpDown::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    click();
}

void numUpDown::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    select();
    emit changed(num);
}

void numUpDown::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    deselect();
}

void numUpDown::setText()
{
    text->setPlainText(array[curr]);

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

    if (curr < count-1)
        curr++;
    else
        curr = 0;

    setText();

    emit clicked();
}
