#ifndef BULLET
#define BULLET

#include <QGraphicsRectItem>
#include <QObject>
#include <QList>
#include <QPixmap>

class Bullet: public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
private:

public:
    Bullet();
    void rotate();
    int speed;
    int degree;
    QString image;
    int pixsize;

    float sx;
    float sy;

    QPixmap rotatePix(QPixmap pix, int deg);
    QGraphicsPixmapItem *pm;

public slots:
    void move();
    void anim();
};

#endif // BULLET

