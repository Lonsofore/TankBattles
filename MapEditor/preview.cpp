#include "preview.h"
#include "ui_preview.h"
#include "QMessageBox"
#include "QGraphicsScene"
#include "QGraphicsView"
#include "block.h"
#include "qdebug.h"
QGraphicsScene * scene;
int xBlocks;
int yBlocks;
int spawns;
preview::preview(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::preview)
{
    ui->setupUi(this);
}

void preview::RecieveValue(int itm)
{
    citm = itm;
}

void preview::RecieveSize(int x, int y)
{
    xsize = x;
    ysize = y;
}

preview::~preview()
{
    delete ui;
}

//При вызове формы изменяется заголовок и активируется | это событие, в этом событии происходит отрисовка
//                                                     V
void preview::on_preview_windowTitleChanged(const QString &title)
{
    emit GetSize();
    //int w = 800;
    //int h = 600;
    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setBackgroundBrush(QBrush(QColor(230,230,230,255)));
    yBlocks = ysize;
    xBlocks = xsize;
    QString img;
    int width = 60; // размер блоков
    int height = 60;
    int x,y;
    int itm;
    scene->setSceneRect(0,0,xBlocks*width,yBlocks*height); // разрешение сцены
    int num = 0;
    for (int i = 0; i < xBlocks; i++)
    {
        for (int j = 0; j < yBlocks; j++)
        {
            emit GetData(i, j);
            itm = citm;
            if (itm != 0)
            {
                Block *block = new Block(itm);
                x = width * j;
                y = height * i;
                if (itm == 1) img = ":/textures/breakable.png";
                if (itm == 2) img = ":/textures/unbreakable.png";
                if (itm == 3) img = "";
                block->setPixmap(QPixmap(img).scaled(width,height));
                block->setPos(x,y);
                scene->addItem(block);
                num++;
            }

        }
    }
}
