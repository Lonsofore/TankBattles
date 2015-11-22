#ifndef TANK
#define TANK

#include <QGraphicsRectItem>
#include <QObject>
#include <QGraphicsItem>
#include <QMediaPlayer>
#include <QPixmap>
#include <QMouseEvent>

class Tank: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Tank();
    void keyPressEvent(QKeyEvent *event);
    void rotate();
    void hrotate();

    QGraphicsPixmapItem *head;

    int degree;  // base degree
    int hdegree; // head degree
    int speed;   // move speed
    int rspeed;  // rotate speed
    int hspeed;  // head rotate speed
    QString baseImage;  // base image
    QString headImage;  // head image
public slots:
    void spawn();
private:
    QMediaPlayer *bulletsound;
};

#endif // TANK

