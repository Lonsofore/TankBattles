#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>

class TextBox:public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    TextBox(int n, int m, int x, int y, QString def=NULL, QGraphicsItem* parent=NULL);
    void keyPressEvent(QKeyEvent * event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    int num;
    int height;
    int width;
    int len;
    int maxLen;

    QString str;

    void update();

    void select();
    void deselect();
signals:
    void changed(int n);
    void back();
private:
    QGraphicsTextItem* text;
};

#endif // TEXTBOX_H
