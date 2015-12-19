#include "preview.h"
#include "ui_preview.h"
#include "QMessageBox"
#include "QGraphicsScene"
#include "QGraphicsView"
#include "QCloseEvent"
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
    scene = new QGraphicsScene();
    ui->preView->setScene(scene);
    ui->preView->setBackgroundBrush(QBrush(QColor(230,230,230,255)));
    ui->preView->viewport()->installEventFilter(this);
    ui->preView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    ui->preView->setResizeAnchor(QGraphicsView::AnchorViewCenter);
    yBlocks = ysize;
    xBlocks = xsize;
    QString img;
    int width = 300; // размер блоков
    int height = 300;
    int x,y;
    int itm;
    scene->setSceneRect(0,0,yBlocks*width,xBlocks*height); // разрешение сцены
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
                if (itm == 3) img = ":/textures/flag.png";
                block->setPixmap(QPixmap(img).scaled(width,height));
                block->setPos(x,y);
                scene->addItem(block);
                num++;
            }

        }
    }
    ui->preView->scale(0.4, 0.4);
}

void preview::closeEvent(QCloseEvent *ev)
{
    ui->preView->deleteLater();
    ui->centralwidget->deleteLater();
    ev->accept();
}

bool preview::eventFilter(QObject *object, QEvent *event)
 {
     if (object == ui->preView->viewport() && event->type() == QEvent::Wheel)
     {
         QWheelEvent *we = static_cast<QWheelEvent*>(event);
         if (we->delta() > 0) ui->preView->scale(1.1, 1.1);
         if (we->delta() < 0) ui->preView->scale(0.9, 0.9);
         return true;
     }
     return false;
 }
