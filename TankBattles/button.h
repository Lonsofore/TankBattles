#ifndef BUTTON
#define BUTTON

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>

class Button:public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Button(int n, QString text, int x, int y, QGraphicsItem* parent=NULL);
    void keyPressEvent(QKeyEvent * event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);

    QString name;
    int height;
    int width;
    int num;

    void select();
    void deselect();
    void click();

    void delay(int millisecondsToWait); // функция задержки
signals:
    void clicked();
};

#endif // BUTTON

