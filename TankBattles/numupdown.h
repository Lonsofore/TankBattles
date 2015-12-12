#ifndef NUMUPDOWN_H
#define NUMUPDOWN_H

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>

class numUpDown:public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    numUpDown(int n, QString arr[], int c, int def, int x, int y, QGraphicsItem* parent=NULL);
    void keyPressEvent(QKeyEvent * event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    QString array[11];
    int height;
    int width;
    int num;
    int curr;
    int count;

    void setText();

    void select();
    void deselect();
    void click();

    QGraphicsTextItem* text;
signals:
    void changed(int n);
    void clicked();
    void back();
private:
};

#endif // NUMUPDOWN_H
