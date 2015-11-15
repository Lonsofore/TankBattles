#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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


    void on_apply_button_clicked();

    void on_field_cellClicked(int row, int column);

    void on_Save_triggered();

    void on_scale_slide_sliderMoved(int position);

    void on_Load_triggered();

    void on_New_triggered();

    void on_Help_triggered();

    void on_About_triggered();

    void on_preview_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
