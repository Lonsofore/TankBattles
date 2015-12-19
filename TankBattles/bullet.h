#ifndef BULLET
#define BULLET

#include <QGraphicsRectItem>
#include <QObject>
#include <QList>
#include <QPixmap>
#include "tank.h"

class Bullet: public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
private:

public:
    Bullet(QGraphicsPixmapItem *parent = 0);

    int dmg;

    void rotate();
    QPixmap rotatePix(QPixmap pix, int deg);

    int speed;
    int degree;

    int dist;
    int maxDist;

    QString image;
    QString empty;
    int pixsize;

    QTimer *timer;
    QTimer *timerf;
    QTimer *timerf1;

    Tank *tank;
    QGraphicsPixmapItem *pm;
    QGraphicsPixmapItem *pm1;

public slots:
    void move();
    void anim();
    void anim1();
};

#endif // BULLET

