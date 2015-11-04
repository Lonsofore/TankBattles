#include "tank.h"

tank::tank()
{
    x = 100;
    y = 100;
    deg = 0;
    speed = 10;
    dmg = 50;
    img = "/img/tank.png";
    weight = 100;
    height = 100;
}

tank::tank(int x1, int y1, int deg1, int speed1, int dmg1, QString img1)
{
    x = x1;
    y = y1;

    //если вдруг угол больше, чем 360
    while (deg1 > 360)
        deg1 -= 360;
    deg = deg1;

    speed = speed1;
    dmg = dmg1;
    img = "/img/" + img1;

    weight = weight1;
    height = height1;
}
