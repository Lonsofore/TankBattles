#ifndef BUTTON
#define BUTTON

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>

class Button:public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Button(QString name, int x, int y, QGraphicsItem* parent=NULL);
    QString name;
    int height;
    int width;

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    void delay(int millisecondsToWait); // функция задержки
signals:
    void clicked();
};

#endif // BUTTON

