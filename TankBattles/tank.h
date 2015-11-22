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
    void keyPressEvent(QKeyEvent *event); // кнопки
    void keyReleaseEvent(QKeyEvent *event);

    // вызов действий
    void onKey();
    void moveForward();
    void moveBack();
    void rotateRight();
    void rotateLeft();
    void headRight();
    void headLeft();
    void fire();

    void rotate(); // поворот платформы
    int degree;    // угол платформы
    int speed;     // скорость
    int rspeed;    // скорость поворота
    double xfix;    // суммирует дробные значения координат
    double yfix;    // x и y
    QString baseImage;  // изображение платформы

    QGraphicsPixmapItem *head; // башня
    void hrotate(); // поворот башни
    int hdegree;    // угол башни
    int hspeed;     // скорость поворота башни
    QString headImage;  // изображение башни

public slots:
    void spawn();

private:
    QMediaPlayer *bulletsound;
    // соответственно с объявленными функциями для действий
    bool mf, mb, rr, rl, hr, hl, fr;
};

#endif // TANK

