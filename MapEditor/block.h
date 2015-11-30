#ifndef BLOCK_H
#define BLOCK_H

#include <QObject>
#include <QGraphicsItem>
#include <QPixmap>

class Block: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Block();
    Block(int n);
    int num;
};

#endif // BLOCK_H
