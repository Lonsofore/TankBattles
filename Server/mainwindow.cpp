#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include <QtNetwork>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    x=10;
    y=60;
    ui->setupUi(this);
}
void MainWindow::on_toolButton_5_clicked()
{
    udpSocket = new QUdpSocket(this);

}
void MainWindow::startBroadcasting()
{
    //udpSocket = new QUdpSocket(this);
    //startButton->setEnabled(false);
    //timer->start(1000);
}

void MainWindow::broadcastDatagram()
{
        ui->label->setText(QString::number(x));
    ui->label_2->setText(QString::number(y));
    QByteArray datagram = QByteArray::number(x) + " " + QByteArray::number(y);
    udpSocket->writeDatagram(datagram.data(), datagram.size(),
                             QHostAddress::Broadcast, 45454);
    x = x;
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_toolButton_4_clicked()
{
    x = x+5;
    ui->label->setText(QString::number(x));
    broadcastDatagram();
}

void MainWindow::on_toolButton_2_clicked()
{

    y= y+5;
    ui->label_2->setText(QString::number(y));
    broadcastDatagram();
}

void MainWindow::on_toolButton_3_clicked()
{
    x=x-5;
    broadcastDatagram();
}

void MainWindow::on_toolButton_clicked()
{
    y=y-5;
    broadcastDatagram();
}


