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
public slots:
    void move();
};

#endif // BULLET

