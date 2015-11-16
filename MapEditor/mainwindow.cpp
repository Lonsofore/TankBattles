#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

int spwncnt = 0;  //Кол-во точек спавна

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->field->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->field->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->field->setIconSize(QSize(69, 65));
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_apply_button_clicked()
{
    ui->field->setColumnCount(ui->xspin->value());
    ui->field->setRowCount(ui->yspin->value());
    for (int i = 0; i<ui->field->rowCount(); i++)  //Заполнение пустых блоков нулями
    {
        for (int j = 0; j<ui->field->columnCount(); j++)
        {   QTableWidgetItem* item = ui->field->item(i,j);
            if (!item || item->text().isEmpty()) //Если пусто
            {
                QTableWidgetItem *insert = new QTableWidgetItem;
                insert->setText("0");
                ui->field->setItem(i, j, insert);

            }
        }
    }
}

void MainWindow::on_field_cellClicked(int row, int column)
{
    if (ui->field->item(row, column)->text() == "S") spwncnt--;
    QTableWidgetItem *item = new QTableWidgetItem;
    if (ui->ubr_btn->isChecked())  //Неразрушаеммый
    {   item->setText("2");
        item->setIcon(QIcon(":/textures/not_breakable.jpg"));
        ui->field->setItem(row, column, item);
    }
    if (ui->br_btn->isChecked())   //Разрушаеммый
    {
        item->setText("1");
        item->setIcon(QIcon(":/textures/breakable.jpg"));
        ui->field->setItem(row, column, item);
    }
    if (ui->emp_btn->isChecked()) //Пустой
    {
        item->setText("0");
        ui->field->setItem(row, column, item);
    }
    if (ui->spwn_btn->isChecked()) //Спавн
    {
        if(((row > 0) && (row < ui->field->rowCount()-1)) && ((column > 0) && (column < ui->field->columnCount()-1)))
        {
            if((ui->field->item(row-1,column)->text() == "0") && (ui->field->item(row+1,column)->text() == "0") &&          //Проверка соседних блоков на пустоту
                    (ui->field->item(row,column-1)->text() == "0") && (ui->field->item(row,column+1)->text() == "0") &&
                    (ui->field->item(row-1,column-1)->text() == "0") && (ui->field->item(row-1,column+1)->text() == "0") &&
                    (ui->field->item(row+1,column-1)->text() == "0") && (ui->field->item(row+1,column+1)->text() == "0"))
            {
                item->setText("S");
                ui->field->setItem(row, column, item);
                spwncnt++;
            }
            else
            {
                QMessageBox::information(this, "Ошибка", "Блоки вокруг точки спавна должны быть свободны!");
            }
        }
        else
        {
            QMessageBox::information(this, "Ошибка", "Блоки вокруг точки спавна должны быть свободны!");
        }
    }
}

void MainWindow::on_Save_triggered()
{
    if ((ui->field->rowCount() == 0) || (ui->field->columnCount() == 0))
    {
        QMessageBox::information(this, "Внимание!", "Нечего сохранять!");
    }
    else
    {   if (spwncnt < 2)
        {
            QMessageBox::information(this, "Внимание!", "Недостаточно точек спавна!\nТребуется по крайней мере 2 точки.");
        }
        else
        {
            QString fname = QFileDialog::getSaveFileName(this, tr("Сохранение"), "", tr("Map files (*.map)"));
            QFile file(fname);
            if(!file.open(QIODevice::WriteOnly))
            {
                if (fname != "") {QMessageBox::information(0, "Ошибка", "Не удалось записать в указанный файл");}
            }
            QTextStream out(&file);
            out << ui->field->rowCount() << " " << ui->field->columnCount() << " " << spwncnt  << endl; //Строка с информацией о карте(размер, кол-во точек спавна)
            QString elm;
            for (int i = 0; i<ui->field->rowCount(); i++)
            {
                elm = "";
                for(int j = 0; j<ui->field->columnCount(); j++)
                {
                    elm = elm + ui->field->item(i, j)->text() + " "; //Запись в переменную строки i+1
                }
                QTextStream out(&file);
                out << elm << endl;  //Запись строки в файл
            }
        }
    }
}



void MainWindow::on_Load_triggered()
{
    if ((ui->field->rowCount() != 0) && (ui->field->columnCount() != 0)) //Если что-то открыто
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Внимание!", "Сохранить текущий файл?", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {MainWindow::on_Save_triggered();}
        else
        {
            ui->field->clearContents();
            ui->field->clearContents();
            ui->field->setRowCount(0);
            ui->field->setColumnCount(0);
        }
    }
    QString fname = QFileDialog::getOpenFileName(this, tr("Загрузка"), "", tr("Map files (*.map)"));
    QFile file(fname);
    if(!file.open(QIODevice::ReadOnly))
    {
        if (fname != "") {QMessageBox::information(this, "Ошибка", "Не удалось прочитать указанный файл");}
    }
    else
    {
        QTextStream in(&file);
        QString line = in.readLine();
        QStringList fields = line.split(" ");
        if (fields.count() == 3)
        {
            QString cnt = fields.at(0);
            int rowcnt = cnt.toInt();  //Кол-во строк
            cnt = fields.at(1);
            int colcnt = cnt.toInt();  //Кол-во столбцов
            cnt = fields.at(2);
            int spwn = cnt.toInt();    //Кол-во точек спавна
            int i, r = 0;
            char elm;                 //Текущий элемент
            spwncnt = 0;
            ui->field->setRowCount(rowcnt);
            ui->field->setColumnCount(colcnt);
            while(!in.atEnd())  //Пока не конец файла
            {
                QString line = in.readLine(); //Читаем строку
                QStringList fields = line.split(" "); //Элементы в файле разделены пробелами
                if (fields.count() == colcnt+1)       //Если кол-во элементов в строке равно кол-ву элементов указанных в строке информации
                {
                    for(i = 0; i < colcnt; i++)
                    {
                       cnt = fields.at(i);
                       elm = cnt.at(0).toLatin1(); //Читаем элемент
                       QTableWidgetItem *item = new QTableWidgetItem;
                       if (elm == '2') //Неразрушаеммый
                       {
                           item->setText("2");
                           item->setIcon(QIcon(":/textures/not_breakable.jpg"));
                           ui->field->setItem(r, i, item);
                       }
                       if (elm == '1') //Разрушаеммый
                       {
                           item->setText("1");
                           item->setIcon(QIcon(":/textures/breakable.jpg"));
                           ui->field->setItem(r, i, item);
                       }
                       if (elm == '0') //Пустой
                       {
                           item->setText("0");
                           ui->field->setItem(r, i, item);
                       }                //TODO: Сделать проверку на заполненность соседних блоков
                       if (elm == 'S') //Спавн
                       {
                           item->setText("S");
                           ui->field->setItem(r, i, item);
                           spwncnt++;
                       }
                       if ((elm != '2') && (elm != '1') && (elm != '0') && (elm != 'S')) //Что-то другое
                       {
                           goto err; //Выдаем сообщение об ошибке и стираем всё
                       }
                    }
                }
                else
                {
                    err: //Выдаем сообщение об ошибке и стираем всё
                    QMessageBox::critical(this, "Ошибка загрузки", "Файл не может быть загружен!\nВозможно он поврежден.");
                    ui->field->clearContents();
                    ui->field->setRowCount(0);
                    ui->field->setColumnCount(0);
                    break;
                }
                r = r+1; //Строка прочитанна
            }
            file.close();
            if ((r != rowcnt) || (spwn != spwncnt)) //Если кол-во строк или точек спавна не равно указанному в начале файла кол-ву
            {
                QMessageBox::critical(this, "Ошибка загрузки", "Файл не может быть загружен!\nВозможно он поврежден.");
                ui->field->clearContents();
            }
            else
            {
                ui->xspin->setValue(colcnt);
                ui->yspin->setValue(rowcnt);
            }
        }
        else
        {
            QMessageBox::critical(this, "Ошибка загрузки", "Неверный формат входного файла!\nВозможно он поврежден.");
        }
    }
}

void MainWindow::on_New_triggered()
{
    if ((ui->field->rowCount() != 0) || (ui->field->columnCount() != 0)) //Если что-то открыто
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Внимание!", "Сохранить текущий файл?", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {MainWindow::on_Save_triggered();}
    } //Стираем всё
    ui->field->clearContents(); //Стираем всё
    ui->field->setRowCount(0);
    ui->field->setColumnCount(0);
    ui->xspin->setValue(1);
    ui->yspin->setValue(1);
    spwncnt = 0;
}

void MainWindow::on_Help_triggered()
{
    QMessageBox::information(this, "Краткая справка", "В редактора имеется 4 вида болков:\n"
                                                      "-Неразрушаеммый блок(2)\n"
                                                      "-Разрушаеммый блок(1)\n"
                                                      "-Блок спавна(S)\n"
                                                      "-Пустой блок(0)\n"
                                                      "Требование к карте:\n"
                                                      "-Минимальный размер: 1*1\n"
                                                      "-Минимальное кол-во блоков спавна: 2\n"
                                                      "Предварительный просмотр на данный момент не реализован!");
}

void MainWindow::on_About_triggered()
{
    QMessageBox::information(this, "О программе", "Редактор карт для игры TankBattles V 0.1.6\n"); //Я не знаю что ещё здесь можно написать
}

void MainWindow::on_preview_clicked()
{
    QMessageBox msgBox(QMessageBox::Question, "!!!", "Вот кто тебя просил сюда заходить!?", QMessageBox::Yes | QMessageBox::No); //Почему бы и да?
    msgBox.setButtonText(QMessageBox::Yes, trUtf8("Я нечаянно"));
    msgBox.setButtonText(QMessageBox::No, trUtf8("Я больше так не буду"));
    msgBox.exec();
}

void MainWindow::on_scale_slide_valueChanged(int value)
{
    ui->field->horizontalHeader()->setDefaultSectionSize(value);
    ui->field->verticalHeader()->setDefaultSectionSize(value);
    if (value > 50)
    {
        ui->field->setIconSize(QSize(value-1, value-5));
    }
}
