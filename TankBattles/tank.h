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
    void defaultTank(); // функция установления стандартных значений (для вызова из класса player)

    // вызов действий для кнопок
    void moveForward(bool check = 0); // движение вперед
    void moveBack(bool check = 0);    // движение назад
    void rotateRight(bool check = 0); // неопределенные функции поворота базы
    void rotateLeft(bool check = 0);  //
    void rotateRight(int deg, bool check = 0); // определенные
    void rotateLeft(int deg, bool check = 0);  //
    void headRight(bool check = 0); // поворот башни направо
    void headLeft(bool check = 0);  // и налево
    void fire(); // выстрел (ограничений между выстрелами нет!)
    int bulletSpeed; // скорость пули
    void randomSpawn();

    // размер танка
    int pixsize;
    void changeSize(int n); // изменить размер
    bool isCollide(); // не касается ли чего-то

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
};

#endif // TANK

