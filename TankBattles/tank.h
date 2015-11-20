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
    Tank(QGraphicsItem * parent=0);
    void keyPressEvent(QKeyEvent *event);

    void rotate();
    int degree;
    int speed;
    int rspeed;
    QString image;
public slots:
    void spawn();
private:
    QMediaPlayer *bulletsound;
};

#endif // TANK

