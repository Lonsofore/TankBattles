#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QLabel;
class QPushButton;
class QTimer;
class QUdpSocket;
QT_END_NAMESPACE
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_toolButton_4_clicked();

    void on_toolButton_2_clicked();

    void on_toolButton_3_clicked();

    void on_toolButton_clicked();

    void on_toolButton_5_clicked();

private:
    Ui::MainWindow *ui;
    QUdpSocket *udpSocket;
    void startBroadcasting();
    void broadcastDatagram();
    int x, y;
};

#endif // MAINWINDOW_H
