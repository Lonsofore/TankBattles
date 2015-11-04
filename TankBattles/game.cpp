#include "game.h"
#include "ui_game.h"
#include "tank.h"

#include <QLabel>
#include <QPixmap>
#include <QString>
#include <QPushButton>
#include <QMessageBox>
#include <QKeyEvent>

game::game(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::game)
{
    ui->setupUi(this);
}

game::~game()
{
    delete ui;
}

QString img;
QLabel *pic;
int x, y, dir;
int speed;

void game::showEvent(QShowEvent *)
{
    tank t;
    spawntank(t);

    /*
    //x = game.height()-20;
    //y = game.width()/2;
    x = 200;
    y = 100;
    dir = 0;
    speed = 3;
    img = "test.png";

    // задаем pixmap
    QPixmap pix(img);

    // для поворота картинки
    //QMatrix rm;
    //rm.rotate(90);
    //pix = pix.transformed(rm);

    //объявляем label с картинкой
    //QLabel *pic = new QLabel(this);
    pic = new QLabel(this);
    pic->setGeometry(x, y, 200, 200); // x, y, weight, height
    int w = pic->width();               // это для того чтобы картинка
    int h = pic->height();              // масштабировалась по границам
    pic->setAlignment(Qt::AlignCenter);
    //pic->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
    pic->setPixmap(pix);
    pic->show();

    // создание кнопки (пример)
    //QPushButton *pb=new QPushButton(tr("NewButton"),this);
    //pb->setGeometry(20, 20, 80, 80);
    //pb->show();
    */

}

void game::spawntank(tank t)
{
    pic = new QLabel(this);
    pic->setGeometry(t.x, t.y, t.weight*1.2, t.height*1.2); // x, y, weight, height
    pic->setAlignment(Qt::AlignCenter);
    pic->setPixmap(QPixmap(t.img));
    pic->show();
}

void game::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
        case Qt::Key_Up:    // ехать вперед

            break;
        case Qt::Key_Down:  // ехать назад

            break;
        case Qt::Key_Left:  // поворот влево

            break;
        case Qt::Key_Right: // поворот вправо

            break;
        case Qt::Key_Escape:// выход

            break;
        case Qt::Key_Space: // выстрел

            break;
        default:
            break;
    }
/*
    if (k != dir)
    {
        QPixmap pix(img);
        QMatrix rm;
        rm.rotate(dir);
        pix = pix.transformed(rm);
        //pic->setPixmap(pix.scaled(100,100,Qt::KeepAspectRatio));
        pic->setPixmap(pix);
    }

    pic->setGeometry(x, y, 200, 200);

    QMessageBox msgBox;
    QString msg;
    msg.setNum(x);
    msgBox.setText(msg);
    msgBox.exec();
    */
}
