#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "preview.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QCloseEvent>
#include <QShortcut>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    void LoadMap(QString fname);

    ~MainWindow();
    //const int getelem(int row, int column);

public slots:

    void GetItem(int row, int column);

    void GetSize();

    //void GetItem(int row, int column);

private slots:

    void on_apply_button_clicked();

    void on_Save_triggered();

    void on_Load_triggered();

    void on_New_triggered();

    void on_Help_triggered();

    void on_About_triggered();

    void on_preview_clicked();

    void on_scale_slide_valueChanged(int value);

    void on_selection_comboBox_currentIndexChanged(int index);

    void on_br_btn_clicked();

    void on_ubr_btn_clicked();

    void on_emp_btn_clicked();

    void on_spwn_btn_clicked();

    void on_field_itemSelectionChanged();

    void on_Exit_triggered();

    void Selection_shortcut();

    void fill(int r, int c);

signals:
    void ReturnValue(int itm);

    void ReturnSize(int x, int y);

private:
    Ui::MainWindow *ui;

    void closeEvent(QCloseEvent *);


    bool isChanged, isFilling;

    int spwncnt;  //Кол-во точек спавна

    int max_size;

    int fillCnt;

    void fill_sub(int, int);

    preview *Preview;

    QShortcut *selShort;

};

#endif // MAINWINDOW_H
