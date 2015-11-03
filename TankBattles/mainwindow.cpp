#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "game.h"
#include <QLabel>
#include <QPixmap>
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    hide();
    game* newgame = new game();
    newgame->show();

    /*
    // создаем новую форму
    QWidget* form = new QWidget();
    form->setAttribute(Qt::WA_DeleteOnClose, true);
    form->show();

    // чтобы узнть, где вообще находимся (чтобы картинку приделать)
    //QString dirname = QFileDialog::getExistingDirectory(form,tr("Select a Directory"),QDir::currentPath());

    // задаем pixmap
    QPixmap pix("test.png");

    // для поворота картинки
    QMatrix rm;
    rm.rotate(90);
    pix = pix.transformed(rm);

    //объявляем label с картинкой
    QLabel *pic = new QLabel(form);
    pic->setGeometry(20, 20, 100, 100); // x, y, weight, height
    int w = pic->width();               // это для того чтобы картинка
    int h = pic->height();              // масштабировалась по границам
    pic->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
    pic->show();
    */


    // создание кнопки (пример)
    //QPushButton *pb=new QPushButton(tr("NewButton"),form);
    //pb->setGeometry(20, 20, 80, 80);
    //pb->show();
}
