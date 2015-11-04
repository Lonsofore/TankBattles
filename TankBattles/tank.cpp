#include "tank.h"

tank::tank()
{
    /*
    x = 100;
    y = 100;
    deg = 0;
    speed = 10;
    dmg = 50;
    img = "img/tank.png";
    weight = 100;
    height = 100;
    */

    QImage im("img/tank.png");
    this->setPixmap(QPixmap::fromImage(im));
    QRect rc;
    rc.setX(100);
    rc.setY(100);
    rc.setWidth(im.width()*1.2);
    rc.setHeight(im.height()*1.2);
    this->setGeometry(rc);
    this->setAlignment(Qt::AlignCenter);
    this->setDegree(0);
}

int tank::getDegree()
{
    return degree;
}

void tank::setDegree(int value)
{
    rotor(value);
}

/*
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
    img = "img/" + img1;

    weight = weight1;
    height = height1;
}
*/

void tank::rotor(int a)
{
    degree=a;
    if (mem_pix==0) mem_pix = new QPixmap(*this->pixmap());
    QPixmap p(rotation(a, *mem_pix));
    QSize sz;
    QRect rc;
    QPoint pn;
    rc=this->geometry();
    pn.setX(rc.x()+rc.width()/2);
    pn.setY(rc.y()+rc.height()/2);
    sz=p.size();
    rc.setWidth(sz.width());
    rc.setHeight(sz.height());
    rc.setX(pn.x()-rc.width()/2);
    rc.setY(pn.y()-rc.height()/2);
    //this->setGeometry(rc);
    this->setPixmap(p);
}

QPixmap tank::rotation(int a, QPixmap pix)
{
    double g=a*PI/180;
    QSize sz=pix.size();
    QPixmap canv_pix(sz*2);
    canv_pix.fill(Qt::transparent); // залить пустотой
    // центр холста
    int x=sz.width();
    int y=sz.height();
    QPainter p(&canv_pix);
    p.translate(x,y);
    p.rotate(a);
    p.translate(-x,-y);
    p.drawPixmap(x/2,y/2, pix);
    p.end();
    int h=x*fabs(sin(g))+ y*fabs(cos(g));
    int w=x*fabs(cos(g))+ y*fabs(sin(g));
    x=x-w/2;
    y=y-h/2;
    pix=canv_pix.copy(x, y, w, h);
    return pix;
}
