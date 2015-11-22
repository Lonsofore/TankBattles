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
};

#endif // TANK

