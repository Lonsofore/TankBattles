#ifndef NUMUPDOWN_H
#define NUMUPDOWN_H

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>

class numUpDown:public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    numUpDown(QString arr[], int n, int x, int y, QGraphicsItem* parent=NULL);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    QString array[11];
    int height;
    int width;
    int num;

    void setText();

    void select();
    void deselect();
    void click();

    QGraphicsTextItem* text;

    void delay(int millisecondsToWait); // функция задержки
signals:
    void clicked();
    void entered(int n);
private:
};

#endif // NUMUPDOWN_H
