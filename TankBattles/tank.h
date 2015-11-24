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
    int keyDelay; // задержка между проверками кнопок
    void keyPressEvent(QKeyEvent *event);   // считывание нажатий
    void keyReleaseEvent(QKeyEvent *event); // и отжатий
    void onKey(int acc); // если кнопка нажата - тикер
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
public slots:
    void spawn();

private:
    QMediaPlayer *bulletsound;
    QMediaPlayer *bulletready;
    // соответственно с объявленными функциями для действий
    bool mf, mb, rr, rl, hr, hl, fr;
};

#endif // TANK

