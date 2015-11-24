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
    // вызов действий для кнопок
    void moveForward();
    void moveBack();
    void rotateRight();
    void rotateLeft();
    void headRight();
    void headLeft();
    void fire();
    bool fireReady; // можно ли уже стрелять
    int fireTime;   // время между выстрелами
    int bulletSpeed;

    int boost;  // ускорение в начале движения
    int iboost; // по сколько отнимать от ускорения

    void rotate();  // поворот платформы
    int degree;     // угол платформы
    int speed;      // скорость
    int rspeed;     // скорость поворота
    double xfix;    // суммирует дробные значения координат
    double yfix;    // x и y
    QString baseImage;  // изображение платформы

    QGraphicsPixmapItem *head; // башня
    void hrotate(); // поворот башни
    int hdegree;    // угол башни
    int hspeed;     // скорость поворота башни
    QString headImage;  // изображение башни

    void delay(int millisecondsToWait); // функция задержки

    bool mf, mb, rr, rl, hr, hl, fr;

    QMediaPlayer *bulletsound;
    QMediaPlayer *bulletready;
};

#endif // TANK

