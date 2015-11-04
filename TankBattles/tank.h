#ifndef TANK_H
#define TANK_H

#include <QString>
#include <QLabel>

class tank : public QLabel
{
public:
    tank(QWidget *parent = 0);
    //tank(int x1, int y1, int deg1, int speed1, int dmg1, QString img1, int weight1, int height1);

    int  getDegree();
    void setDegree(int value);

    /*
    int x, y;    // координаты
    int degree;     // угол поворота в градусах
    int speed;   // скорость перемещения
    int dmg;     // урон
    QString img; // картинка танка
    int weight;  //
    int height;  // размеры танка
    */

    QPixmap rotation(int a, QPixmap pix);
public slots:
    void rotor(int a);
};

#endif // TANK_H
