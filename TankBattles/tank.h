#ifndef TANK
#define TANK

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
    void defaultTank(int x, int y); // функция установления стандартных значений (для вызова из класса player)

    // вызов действий для кнопок
    void moveForward(); // движение вперед
    void moveBack();    // движение назад
    void rotateRight(); // неопределенные функции поворота базы
    void rotateLeft();  //
    void rotateRight(int deg); // определенные
    void rotateLeft(int deg);  //
    void headRight(); // поворот башни направо
    void headLeft();  // и налево
    void fire(); // выстрел (ограничений между выстрелами нет!)
    int bulletSpeed; // скорость пули

    // размер танка
    int pixsize;

    // платформа
    void rotate();  // поворот платформы
    int degree;     // угол платформы
    int speed;      // скорость
    int rspeed;     // скорость поворота
    double xfix;    // суммирует дробные значения координат
    double yfix;    // x и y
    QString baseImage;  // изображение платформы

    // башня
    QGraphicsPixmapItem *head; // башня
    void hrotate(); // поворот башни
    int hdegree;    // угол башни
    int hspeed;     // скорость поворота башни
    QString headImage;  // изображение башни

    // звуки
    QMediaPlayer *bulletsound; // выстрел
    QMediaPlayer *bulletready; // перезарядка
    QMediaPlayer *tankhrotate; // поворот башни

    void delay(int millisecondsToWait); // функция задержки
};

#endif // TANK

