#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include <QtNetwork>
#include <QMessageBox>
#include <QDebug>
static inline QByteArray IntToArray(qint32 source);

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
    //udpSocket = new QUdpSocket(this);
    tcpSocket = new QTcpSocket();
    tcpSocket->connectToHost("192.168.1.3", 7);
    tcpSocket->waitForConnected(3000000);//) QMessageBox::information(this, "Connection refused", "Connection timed out!");
}
void MainWindow::startBroadcasting()
{
    //udpSocket = new QUdpSocket(this);
    //startButton->setEnabled(false);
    //timer->start(1000);
}

void MainWindow::broadcastDatagram()
{
    if(tcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        QByteArray data = QByteArray::number(x) + ' ' + QByteArray::number(y);
        tcpSocket->write(IntToArray(data.size())); //write size of data
        tcpSocket->write(data); //write the data itself
        tcpSocket->waitForBytesWritten();
        return;
    }
    else
        QMessageBox::information(this, "ERROR", "Connection error");
        return ;//false;
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
    ui->label->setText(QString::number(x));
    broadcastDatagram();
}

void MainWindow::on_toolButton_clicked()
{
    y=y-5;
    ui->label_2->setText(QString::number(y));
    broadcastDatagram();
}

QByteArray IntToArray(qint32 source)
{
    //Avoid use of cast, this is the Qt way to serialize objects
    QByteArray temp;
    QDataStream data(&temp, QIODevice::ReadWrite);
    data << source;
    return temp;
}
