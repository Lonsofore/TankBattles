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
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    int num;
    int height;
    int width;

    void setText(QString name);

    void select();
    void deselect();
    void click();

    void delay(int millisecondsToWait); // функция задержки
signals:
    void changed(int n);
    void entered(int n);
    void clicked();
private:
    QGraphicsTextItem* text;
};

#endif // BUTTON

