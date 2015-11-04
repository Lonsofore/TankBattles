#include "game.h"
#include "ui_game.h"
#include "tank.h"

#include <QLabel>
#include <QPixmap>
#include <QString>
#include <QPushButton>
#include <QMessageBox>
#include <QKeyEvent>

tank *t1;

game::game(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::game)
{
    ui->setupUi(this);
    spawntank();
}


game::~game()
{
    delete ui;
}

void game::spawntank()
{
    /*
    pic = new QLabel(this);
    pic->setGeometry(t.x, t.y, t.weight*1.2, t.height*1.2); // x, y, weight, height
    pic->setAlignment(Qt::AlignCenter);
    pic->setPixmap(QPixmap(t.img));
    pic->show();
    */

    t1=new tank(this);
    t1->show();
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
}
