#include "textbox.h"
#include <QGraphicsPixmapItem>
#include <QTime>
#include <QCoreApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QFontDatabase>
#include <QKeyEvent>

TextBox::TextBox(int n, int m, int x, int y, QString def, QGraphicsItem *parent) : QGraphicsPixmapItem(parent)
{
    num = n;
    width = x;
    height = y;
    maxLen = m;

    QString image = ":/images/images/menu/Shape.png";
    setPixmap(QPixmap(image).scaled(width,height));

    // добавление шрифта
    QFontDatabase::addApplicationFont(":/fonts/fonts/GOTHIC.TTF");

    // текст
    text = new QGraphicsTextItem(def,this);
    text->setDefaultTextColor(QColor(100, 100, 100, 255));
    QFont font("Century Gothic",38);
    text->setFont(font);
    int xPos = this->width/2 - text->boundingRect().width()/2;
    int yPos = this->height/2 - text->boundingRect().height()/2 - 5;
    text->setPos(xPos,yPos);

    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsFocusable); // кнопку можно выделить (для нажатий клавиш)
}

void TextBox::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Up:
            emit changed(num-1);
        break;

        case Qt::Key_Down:
            emit changed(num+1);
        break;

        case Qt::Key_Backspace:
            if (str.length() > 0)
            {
                str.remove(str.length()-1, 1);
                update();
            }
        break;

        case Qt::Key_Escape: // Esc
            emit back();
        break;

        default:
            if (str.length() < maxLen)
            {
                str.append(event->text());
                update();
            }
    }
}

void TextBox::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    select();
    emit changed(num);
}

void TextBox::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    deselect();
}

void TextBox::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
}

void TextBox::update()
{
    text->setDefaultTextColor(QColor(71, 71, 71, 255));
    text->setPlainText(str);
    int xPos = this->width/2 - text->boundingRect().width()/2;
    int yPos = this->height/2 - text->boundingRect().height()/2 - 5;
    text->setPos(xPos,yPos);
}

void TextBox::select()
{
    QString image = ":/images/images/menu/Select.png";
    setPixmap(QPixmap(image).scaled(width,height));
    setFocus();
}

void TextBox::deselect()
{
    QString image = ":/images/images/menu/Shape.png";
    setPixmap(QPixmap(image).scaled(width,height));
}





















