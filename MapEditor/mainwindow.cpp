#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "preview.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/textures/editor.ico"));
    max_size = 100;
    spwncnt = 0;
    isChanged = 0;
    QPalette p = ui->field->palette();
    p.setColor(QPalette::Base, QColor(229, 229, 229, 255));
    ui->field->setPalette(p);
    ui->field->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->field->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->field->setIconSize(QSize(69, 65));
    if (QSysInfo::buildCpuArchitecture() == "i386") max_size = 300;
    if (QSysInfo::buildCpuArchitecture() == "x86_64") max_size = 600;
    ui->xspin->setMaximum(max_size);
    ui->yspin->setMaximum(max_size);
    selShort = new QShortcut(QKeySequence("Ctrl+E"), this);
    connect(selShort, SIGNAL(activated()), this, SLOT(Selection_shortcut()));
    connect(ui->field, SIGNAL(cellClicked(int,int)), this, SLOT(fill(int,int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_apply_button_clicked()
{
    isChanged = 1;
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

void MainWindow::on_Save_triggered()
{
    if ((ui->field->rowCount() == 0) || (ui->field->columnCount() == 0))
    {
        QMessageBox::warning(this, "Внимание!", "Нечего сохранять!");
    }
    else
    {   if (spwncnt < 2)
        {
            QMessageBox::warning(this, "Ошибка построения карты", "Недостаточно точек спавна!\nТребуется по крайней мере 2 точки.");
        }
        else
        {
            for(int column = 0; column < ui->xspin->value(); column++)
            {
                for(int row = 0; row < ui->yspin->value(); row++)
                {
                    if (ui->field->item(row, column)->text() == "S")
                    {
                        if(((row > 0) && (row < ui->field->rowCount()-1)) && ((column > 0) && (column < ui->field->columnCount()-1))) //Если ячейка расположена не на краю
                        {
                            if((ui->field->item(row-1,column)->text() != "0") || (ui->field->item(row+1,column)->text() != "0") ||                 //Проверка соседних элементов на отсутствие в них
                               (ui->field->item(row,column-1)->text() != "0") || (ui->field->item(row,column+1)->text() != "0") ||
                               (ui->field->item(row-1,column-1)->text() != "0") || (ui->field->item(row-1,column+1)->text() != "0") ||
                               (ui->field->item(row+1,column-1)->text() != "0") || (ui->field->item(row+1,column+1)->text() != "0"))
                            {
                                QMessageBox::warning(this, "Ошибка построения карты", "Блоки вокруг спавна должны быть свободны!"); return;
                            }
                        }
                        else {QMessageBox::warning(this, "Ошибка построения карты", "Блок спавна не может располагаться на краю карты!"); return;}
                   }
               }
            }
            QString fname = QFileDialog::getSaveFileName(this, tr("Сохранение"), "Map_Name.map", tr("Map files (*.map)"));
            if (fname.split('.').count() < 2) fname +=".map";
            else
                if (fname.split('.').at(1) != "map") fname += ".map";
            QFile file(fname);
            if(!file.open(QIODevice::WriteOnly))
            {
                if (fname != "") {QMessageBox::information(this, "Ошибка", "Не удалось записать в указанный файл");}
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
            isChanged = 0;
        }
    }
}

void MainWindow::on_Load_triggered()
{
    if ((ui->field->rowCount() != 0) && (ui->field->columnCount() != 0)) //Если что-то открыто
    {
        if (isChanged) //Если произошли какие-нибудь изменения
        {
            QMessageBox reply (QMessageBox::Question, "Внимание!", "Сохранить текущий файл?", QMessageBox::Yes|QMessageBox::No);
            reply.setButtonText(QMessageBox::Yes, "Да");
            reply.setButtonText(QMessageBox::No, "Нет");
            if (reply.exec() == QMessageBox::Yes) {MainWindow::on_Save_triggered();}
        }
    }
    ui->field->clearContents();
    ui->field->clearContents();
    ui->field->setRowCount(0);
    ui->field->setColumnCount(0);
    QString fname = QFileDialog::getOpenFileName(this, tr("Загрузка"), (QDir::currentPath() + "/maps"), tr("Map files (*.map)"));
    LoadMap(fname);
}

void MainWindow::on_New_triggered()
{
    if ((ui->field->rowCount() != 0) || (ui->field->columnCount() != 0)) //Если что-то открыто
    {
        if(isChanged) //Если что-либо изменено
        {
            QMessageBox reply (QMessageBox::Question, "Внимание!", "Сохранить текущий файл?", QMessageBox::Yes|QMessageBox::No);
            reply.setButtonText(QMessageBox::Yes, "Да");
            reply.setButtonText(QMessageBox::No, "Нет");
            if (reply.exec() == QMessageBox::Yes) {MainWindow::on_Save_triggered();}
        }
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
    QMessageBox::information(this, "Краткая справка", (QString("В редактора имеется 4 вида болков:\n"
                                                      "-Неразрушаеммый блок(2)\n"
                                                      "-Разрушаеммый блок(1)\n"
                                                      "-Блок спавна(S)\n"
                                                      "-Пустой блок(0)\n"
                                                      "Требование к карте:\n"
                                                      "-Минимальный размер: 5*5\n"
                                                      "-Минимальное кол-во блоков спавна: 2\n"
                                                      "-Максимальный размер: ") + QString::number(max_size) + "*" +QString::number(max_size) + "\n"
                                                      "Некоторые горячие клавиши:\n"
                                                      "-Ctrl+P - Предварительный просмотр\n"
                                                      "-Ctrl+E - Изменить режим выделения\n"
                                                      "-1..4 - Выбрать инструмент"));
}

void MainWindow::on_About_triggered()
{
    QMessageBox::information(this, "О программе", "Редактор карт для игры TankBattles V 1.05\n"); //Я не знаю что ещё здесь можно написать
}

void MainWindow::on_preview_clicked()
{
    bool isSpwnCorrect = 0;
    if ((ui->field->rowCount() == 0) || (ui->field->columnCount() == 0))
    {
        QMessageBox::critical(this, "Ошибка!", "Нечего показывать!");
    }
    else
    {   if (spwncnt < 2)
        {
            QMessageBox::warning(this, "Ошибка построения карты!", "Установленно менее двух точек спавна");
        }
        else
        {
            for(int column = 0; column < ui->xspin->value(); column++)
            {
                for(int row = 0; row < ui->yspin->value(); row++)
                {
                    if (ui->field->item(row, column)->text() == "S")
                    {
                        if(((row > 0) && (row < ui->field->rowCount()-1)) && ((column > 0) && (column < ui->field->columnCount()-1))) //Если ячейка расположена не на краю
                        {
                            if((ui->field->item(row-1,column)->text() != "0") || (ui->field->item(row+1,column)->text() != "0") ||                 //Проверка соседних элементов на отсутствие в них блоков
                               (ui->field->item(row,column-1)->text() != "0") || (ui->field->item(row,column+1)->text() != "0") ||
                               (ui->field->item(row-1,column-1)->text() != "0") || (ui->field->item(row-1,column+1)->text() != "0") ||
                               (ui->field->item(row+1,column-1)->text() != "0") || (ui->field->item(row+1,column+1)->text() != "0"))
                            {
                                QMessageBox::warning(this, "Ошибка построения карты", "Блоки вокруг спавна должны быть свободны!"); return;
                            }
                        }
                        else {QMessageBox::warning(this, "Ошибка построения карты", "Блок спавна не может располагаться на краю карты!"); return;}
                    }
                }
            }
            Preview = new preview(this);
            QObject::connect(Preview, SIGNAL(GetData(int, int)),this, SLOT(GetItem(int,int)));       //Получение данных
            QObject::connect(this, SIGNAL(ReturnValue(int)), Preview, SLOT(RecieveValue(int)));      //Из таблицы (предв. просмотр)
            QObject::connect(Preview, SIGNAL(GetSize()), this, SLOT(GetSize()));                     //Получение
            QObject::connect(this, SIGNAL(ReturnSize(int,int)), Preview, SLOT(RecieveSize(int,int)));//Размер (предв. просмотр)
            Preview->setAttribute(Qt::WA_DeleteOnClose);
            Preview->show();
            Preview->setWindowTitle("Предварительный просмотр");
        }
    }
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

void MainWindow::on_selection_comboBox_currentIndexChanged(int index)
{
    ui->field->clearSelection();
    switch(index)
    {
        case 0:
            ui->field->setSelectionMode(QAbstractItemView::SingleSelection);
            isFilling = 0;
        break;
        case 1:
            ui->field->setSelectionMode(QAbstractItemView::ExtendedSelection);
            isFilling = 0;
        break;
        case 2:
            QMessageBox::information(this, "Внимание!", "Функция работает в тестовом режиме.\nВозможны ошибки при заливке больших областей!");
            ui->field->setSelectionMode(QAbstractItemView::NoSelection);
            isFilling = 1;
        break;
        case 3:
            ui->field->setSelectionMode(QAbstractItemView::NoSelection);
            isFilling = 0;
        break;
        default: break;
    }
}

//Очистка выделения таблицы (В противном случае при изенении
//типа блока и клике по таблице изменяется тип последних выделенных блоков)
void MainWindow::on_br_btn_clicked()
{
    ui->field->clearSelection();
}

void MainWindow::on_ubr_btn_clicked()
{
    ui->field->clearSelection();
}

void MainWindow::on_emp_btn_clicked()
{
    ui->field->clearSelection();
}

void MainWindow::on_spwn_btn_clicked()
{
    ui->field->clearSelection();
}
//Установка блока
void MainWindow::on_field_itemSelectionChanged()
{
    isChanged = 1;
    if(!isFilling)
    {
        int row, column;
        {
            QList<QTableWidgetItem*> selection =  ui->field->selectedItems();
            for (int i = 0; i < selection.count(); ++i)
            {
                QTableWidgetItem *itm = selection.at(i);
                row = itm->row();
                column = itm->column();
                if (ui->field->item(row, column)->text() == "S") spwncnt--;
                QTableWidgetItem *item = new QTableWidgetItem;
                if (ui->ubr_btn->isChecked())  //Неразрушаеммый
                {
                    item->setText("2");
                    ui->field->setItem(row, column, item);
                    ui->field->item(row, column)->setBackgroundColor(QColor(38, 14, 55));
                }
                if (ui->br_btn->isChecked())   //Разрушаеммый
                {
                    item->setText("1");
                    //  item->setIcon(QIcon(":/textures/breakable.png"));
                    ui->field->setItem(row, column, item);
                    ui->field->item(row, column)->setBackgroundColor(QColor(255, 202, 61));
                }
                if (ui->emp_btn->isChecked()) //Пустой
                {
                    item->setText("0");
                    ui->field->setItem(row, column, item);
                }
                if (ui->spwn_btn->isChecked()) //Спавн
                {
                    item->setText("S");
                    ui->field->setItem(row, column, item);
                    spwncnt++;
                }
            }
        }
    }
}
//Возвращает элемент field
void MainWindow::GetItem(int row, int column)
{
    int itm;
    if (ui->field->item(row, column)->text() != "S")
        itm = ui->field->item(row, column)->text().toInt();
    else
        itm = 3;
    emit ReturnValue(itm);
}
//Возвращает кол-во элементов
void MainWindow::GetSize()
{
    int x, y;
    x = ui->field->rowCount();
    y = ui->field->columnCount();
    emit ReturnSize(x, y);
}

void MainWindow::closeEvent(QCloseEvent *ev)
{
    if ((ui->field->rowCount() != 0) && (ui->field->columnCount() != 0)) //Если что-то открыто
    {
        if (isChanged) //Если произошли какие-нибудь изменения
        {
            QMessageBox reply (QMessageBox::Question, "Внимание!", "Сохранить текущий файл?", QMessageBox::Yes|QMessageBox::No);
            reply.setButtonText(QMessageBox::Yes, "Да");
            reply.setButtonText(QMessageBox::No, "Нет");
            if (reply.exec() == QMessageBox::Yes) {MainWindow::on_Save_triggered();}
        }
    }
    ev->accept();
}

void MainWindow::LoadMap(QString fname)
{
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
            if (rowcnt > max_size || colcnt > max_size)
            {
                QMessageBox::critical(this, "Ошибка загрузки", "Файл не может быть загружен!\nСлишком большой размер карты.");
                return;
            }
            else
            {
                if (spwn >= 2 || rowcnt<=0 || colcnt<=0)
                {
                    char elm;                 //Текущий элемент
                    int **spcord = new int* [spwn]; //Массив для записи координат точек спавна для последующей проверки на правильность их расположения
                    for (i = 0; i < spwn; i++)
                    {
                        spcord[i] = new int [2];
                    }
                    spwncnt = 0;
                    ui->field->setRowCount(rowcnt);
                    ui->field->setColumnCount(colcnt);
                    while(!in.atEnd())  //Пока не конец файла
                    {
                        QString line = in.readLine(); //Читаем строкуВнимани
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
                                   item->setBackgroundColor(QColor(38, 14, 55));
                                   ui->field->setItem(r, i, item);
                               }
                               if (elm == '1') //Разрушаеммый
                               {
                                   item->setText("1");
                                   item->setBackgroundColor(QColor(255, 202, 61));
                                   ui->field->setItem(r, i, item);
                               }
                               if (elm == '0') //Пустой
                               {
                                   item->setText("0");
                                   ui->field->setItem(r, i, item);
                               }
                               if (elm == 'S') //Спавн
                               {
                                   item->setText("S");
                                   ui->field->setItem(r, i, item);
                                   spcord[spwncnt][0] = r;
                                   spcord[spwncnt][1] = i;
                                   spwncnt++;
                               }
                               if ((elm != '2') && (elm != '1') && (elm != '0') && (elm != 'S')) //Что-то другое
                               {
                                   QMessageBox::critical(this, "Ошибка загрузки", "Файл не может быть загружен!\nНе удалось распознать символ.");
                                   ui->field->clearContents();
                                   ui->field->setRowCount(0);
                                   ui->field->setColumnCount(0);
                                   for (i = 0; i < spwncnt; i++)
                                   {
                                       delete[] spcord[i];
                                   }
                                   file.close();
                                   return;
                               }
                            }
                        }
                        else
                        {
                            QMessageBox::critical(this, "Ошибка загрузки", "Файл не может быть загружен!\nИнформация в первой строке файла не соответствует действительности."); //Выдаем сообщение об ошибке и стираем всё
                            ui->field->clearContents();
                            ui->field->setRowCount(0);
                            ui->field->setColumnCount(0);
                            for (i = 0; i < spwncnt; i++)
                            {
                                delete[] spcord[i];
                            }
                            file.close();
                            return;
                        }
                        r = r+1; //Строка прочитанна
                        if (r >= rowcnt) break;
                    }
                    for(i = 0; i<spwncnt; i++) //Проверка на правильность расположения точек спавна
                    {
                       if (((spcord[i][0] > 0) && (spcord[i][0] < ui->field->rowCount()-1)) && ((spcord[i][1] > 0) && (spcord[i][1] < ui->field->columnCount()-1)))
                       {
                           if((ui->field->item(spcord[i][0]-1,spcord[i][1])->text() != "0") || (ui->field->item(spcord[i][0]+1,spcord[i][1])->text() != "0") ||          //Проверка соседних блоков на пустоту
                                   (ui->field->item(spcord[i][0],spcord[i][1]-1)->text() != "0") || (ui->field->item(spcord[i][0],spcord[i][1]+1)->text() != "0") ||
                                   (ui->field->item(spcord[i][0]-1,spcord[i][1]-1)->text() != "0") || (ui->field->item(spcord[i][0]-1,spcord[i][1]+1)->text() != "0") ||
                                   (ui->field->item(spcord[i][0]+1,spcord[i][1]-1)->text() != "0") || (ui->field->item(spcord[i][0]+1,spcord[i][1]+1)->text() != "0"))
                           {
                               QMessageBox::critical(this, "Ошибка загрузки", "Файл не может быть загружен!\nБлоки спавна расположенны неправильно.");
                               ui->field->clearContents();
                               ui->field->setRowCount(0);
                               ui->field->setColumnCount(0);
                               file.close();
                               return;
                           }
                       }
                       else
                       {
                           QMessageBox::critical(this, "Ошибка загрузки", "Файл не может быть загружен!\nБлоки спавна расположенны неправильно.");
                           ui->field->clearContents();
                           ui->field->setRowCount(0);
                           ui->field->setColumnCount(0);
                           file.close();
                           return;
                       }
                    }
                }
                else
                {
                    file.close();
                    QMessageBox::critical(this, "Ошибка загрузки", "Файл не может быть загружен!\nПервая строка файла содержит ошибочные значения.");
                    return;
                }
            }
            file.close();
            if ((r != rowcnt) || (spwn != spwncnt)) //Если кол-во строк или точек спавна не равно указанному в начале файла кол-ву
            {
                QMessageBox::critical(this, "Ошибка загрузки", "Файл не может быть загружен!\nИнформация в первой строке файла не соответствует действительности.");
                ui->field->clearContents();
                ui->field->setRowCount(0);
                ui->field->setColumnCount(0);
            }
            else
            {
                ui->xspin->setValue(colcnt);
                ui->yspin->setValue(rowcnt);
                isChanged = 0;
            }
        }
        else
        {
            QMessageBox::critical(this, "Ошибка загрузки", "Неправильный формат входного файла!\nНеправильный формат первой строки файла.");
        }
    }
}

void MainWindow::Selection_shortcut()
{
    int indx = ui->selection_comboBox->currentIndex();
    if (indx < 4) {ui->selection_comboBox->setCurrentIndex(++indx);}
    if (indx == 4) {ui->selection_comboBox->setCurrentIndex(0);}
}

void MainWindow::on_Exit_triggered()
{if (isChanged) //Если произошли какие-нибудь изменения
    {
        QMessageBox reply (QMessageBox::Question, "Внимание!", "Сохранить текущий файл?", QMessageBox::Yes|QMessageBox::No);
        reply.setButtonText(QMessageBox::Yes, "Да");
        reply.setButtonText(QMessageBox::No, "Нет");
        if (reply.exec() == QMessageBox::Yes) {MainWindow::on_Save_triggered();}
    }
    exit(0);
}

void MainWindow::fill(int r, int c)
{
    if (isFilling)
    {
        QString elem = ui->field->item(r, c)->text();
        QTableWidgetItem *item = new QTableWidgetItem;
        if (ui->ubr_btn->isChecked())  //Неразрушаеммый
        {
            if (ui->field->item(r, c)->text() == "2") return;
            item->setText("2");
            ui->field->setItem(r, c, item);
            ui->field->item(r, c)->setBackgroundColor(QColor(38, 14, 55));
        }
        if (ui->br_btn->isChecked())   //Разрушаеммый
        {
            if (ui->field->item(r, c)->text() == "1") return;
            item->setText("1");
            //  item->setIcon(QIcon(":/textures/breakable.png"));
            ui->field->setItem(r, c, item);
            ui->field->item(r, c)->setBackgroundColor(QColor(255, 202, 61));
        }
        if (ui->emp_btn->isChecked()) //Пустой
        {
            if (ui->field->item(r, c)->text() == "0") return;
            item->setText("0");
            ui->field->setItem(r, c, item);
        }
        if (ui->spwn_btn->isChecked()) //Спавн
        {
            if (ui->field->item(r, c)->text() == "S") return;
            item->setText("S");
            ui->field->setItem(r, c, item);
            spwncnt++;
        }
        if (r > 0)
        {
            if(ui->field->item(r-1, c)->text() == elem)
                fill(r-1, c);
        }
        if (r < ui->field->rowCount()-1)
        {
            if(ui->field->item(r+1, c)->text() == elem)
                fill(r+1, c);
        }
        if (c > 0)
        {
            if(ui->field->item(r, c-1)->text() == elem)
                fill(r, c-1);}
        if (c < ui->field->columnCount()-1)
        {
            if(ui->field->item(r, c+1)->text() == elem)
                fill(r, c+1);
        }
        //delete *item;
    }
}
