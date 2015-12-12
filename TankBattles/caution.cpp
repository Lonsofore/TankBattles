#include "caution.h"
#include <QGraphicsPixmapItem>
#include <QTime>
#include <QCoreApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QFontDatabase>

Caution::Caution(QString image, QString text, int t)
{
    timer = t;
    width = 400;
    height = 176;

    setPixmap(QPixmap(image).scaled(width,height));

    // шрифт
    QFontDatabase::addApplicationFont(":/fonts/fonts/msyi.ttf");

    // Caution
    text1 = new QGraphicsTextItem(text,this);
    text1->setDefaultTextColor(QColor(243, 234, 234, 255));
    QFont font1("Microsoft Yi baiti",70);
    font1.setBold(true);
    text1->setFont(font1);
    int xPos = this->width/2 - text1->boundingRect().width()/2;
    int yPos;
    if (timer > 0)
        yPos = this->height/2 - text1->boundingRect().height()+5;
    else
        yPos = this->height/2 - text1->boundingRect().height()/2;
    text1->setPos(xPos,yPos);

    if (timer > 0)
    {
        // шрифт цифр
        QFontDatabase::addApplicationFont(":/fonts/fonts/CITYNL.TTF");

        // цифры
        QString str = "00:0" + QString::number(timer);
        text2 = new QGraphicsTextItem(str,this);
        text2->setDefaultTextColor(QColor(243, 234, 234, 255));
        QFont font2("a_CityNovalt",60);
        font2.setBold(true);
        text2->setFont(font2);
        xPos = this->width/2 - text2->boundingRect().width()/2;
        yPos = this->height/2 - 15;
        text2->setPos(xPos,yPos);
    }
}

void Caution::decTimer()
{
    if (timer > 0)
    {
        timer--;
        QString str = "00:0" + QString::number(timer);
        text2->setPlainText(str);
    }
}
