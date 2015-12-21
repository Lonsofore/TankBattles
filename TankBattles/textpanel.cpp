#include "textpanel.h"
#include <QGraphicsPixmapItem>
#include <QTime>
#include <QCoreApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QFontDatabase>

TextPanel::TextPanel(QString t, QString img, int x, int y)
{
    width = x;
    height = y;

    QString image = img;
    setPixmap(QPixmap(image).scaled(width,height));

    // шрифт
    QFontDatabase::addApplicationFont(":/fonts/fonts/GOTHIC.TTF");

    // текст
    text = new QGraphicsTextItem(t,this);
    text->setDefaultTextColor(QColor(71, 71, 71, 255));
    QFont font("Century Gothic",38);
    text->setFont(font);
    int xPos = this->width/2 - text->boundingRect().width()/2;
    int yPos = this->height/2 - text->boundingRect().height()/2 - 5;
    text->setPos(xPos,yPos);
}

void TextPanel::setText(QString newtext)
{
    text->setPlainText(newtext);
    int xPos = this->width/2 - text->boundingRect().width()/2;
    int yPos = this->height/2 - text->boundingRect().height()/2 - 5;
    text->setPos(xPos,yPos);
}



