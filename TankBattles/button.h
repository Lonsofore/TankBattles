#ifndef BUTTON
#define BUTTON

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>

class Button:public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Button(int n, QString name, int x, int y, QGraphicsItem* parent=NULL);
    void keyPressEvent(QKeyEvent * event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);

    int height;
    int width;
    int num;

    void select();
    void deselect();
    void click();

    void delay(int millisecondsToWait); // функция задержки
signals:
    void clicked();
private:
    QGraphicsTextItem* text;
};

#endif // BUTTON

