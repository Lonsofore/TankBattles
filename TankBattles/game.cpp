#include "game.h"
#include "ui_game.h"
#include <QLabel>
#include <QPixmap>
#include <QString>
#include <QPushButton>
#include <QMessageBox>
#include <QKeyEvent>
#include "panzer.h"

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
    Panzer z;
    //z.spawn();

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


}

void rotateTank(int r)
{

}

void game::keyPressEvent(QKeyEvent* event)
{
    int k = dir;
    switch (event->key())
    {
        case Qt::Key_Up:
            switch (dir)
            {
                case 0:
                    y -= speed;
                case 1:
                    x += speed;
                case 2:
                    y += speed;
                case 3:
                    x -= speed;
            }
            break;
        case Qt::Key_Down:
            y += 3;
            break;
        case Qt::Key_Left:
            dir -= 2;
            break;
        case Qt::Key_Right:
            dir += 2;
            break;
        default:
            break;
    }

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
    /*
    QMessageBox msgBox;
    QString msg;
    msg.setNum(x);
    msgBox.setText(msg);
    msgBox.exec();
    */
}
