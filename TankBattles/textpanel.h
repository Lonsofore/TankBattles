#ifndef TEXTPANEL_H
#define TEXTPANEL_H

#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>

class TextPanel:public QObject, public QGraphicsPixmapItem
{
public:
    TextPanel(QString t, QString img, int x, int y);
    void setText(QString newtext);
    int height;
    int width;
private:
    QGraphicsTextItem* text;
};

#endif // TEXTPANEL_H
