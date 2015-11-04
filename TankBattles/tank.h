#ifndef TANK_H
#define TANK_H

#include <QString>

class tank
{
public:
    tank();
    tank(int x1, int y1, int deg1, int speed1, int dmg1, QString img1, int weight1, int height1);

    int x, y;    // координаты
    int deg;     // угол поворота в градусах
    int speed;   // скорость перемещения
    int dmg;     // урон
    QString img; // картинка танка
    int weight;  //
    int height;  // размеры танка
};

#endif // TANK_H
