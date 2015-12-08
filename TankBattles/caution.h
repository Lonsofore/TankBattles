#ifndef CAUTION_H
#define CAUTION_H

#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>

class Caution:public QObject, public QGraphicsPixmapItem
{
public:
    Caution(QString image, QString text, int t = 0);
    int timer;
    int height;
    int width;
    void decTimer();
private:
    QGraphicsTextItem* text1;
    QGraphicsTextItem* text2;
};

#endif // CAUTION_H
