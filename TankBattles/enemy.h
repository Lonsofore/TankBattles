#ifndef ENEMY
#define ENEMY

#include <QGraphicsRectItem>
#include <QObject>

class Enemy: public QObject,public QGraphicsRectItem
{
    Q_OBJECT
public:
    Enemy();
public slots:
    void move();
};

#endif // ENEMY

