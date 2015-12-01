#ifndef TEXTPANEL_H
#define TEXTPANEL_H

#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>

class TextPanel:public QObject, public QGraphicsPixmapItem
{
public:
    TextPanel(QString t, int x, int y);
    int height;
    int width;
private:
    QGraphicsTextItem* text;
};

#endif // TEXTPANEL_H
