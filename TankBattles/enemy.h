#ifndef ENEMY
#define ENEMY

#include <QGraphicsRectItem>
#include <QObject>

class Enemy: public QObject,public QGraphicsRectItem
{
    Q_OBJECT
public:
    Enemy();
    int test;
public slots:
    void move();
};

#endif // ENEMY

