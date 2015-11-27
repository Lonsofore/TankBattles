#include "game.h"
#include <QTimer>
#include <QGraphicsTextItem>
#include <QFont>
#include "enemy.h"
#include <QMediaPlayer>
#include "button.h"
#include "player.h"
#include <QTime>
#include <QCoreApplication>
#include <QFileDialog>
#include "block.h"

bool started = false;
const int numButton = 4;
Button *menuButtons[numButton];

Game::Game(QWidget *parent){
    int width = 800;
    int height = 600;

    // создание сцены
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,width,height); // разрешение сцены
    setScene(scene);

    // чтобы не появлялись скроллбары
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(width,height); // разрешение экрана

    // всего кнопок 4, текущая выделенная - 0
    numsButtons = numButton;
    curButton = 0;
    pressed = false;

    show();
}

void Game::mouseReleaseEvent(QMouseEvent *event)
{
    if (started)
        player->setFocus();
}

void Game::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::WindowStateChange && started == true)
    {
        player->playerReset();
    }
}

void Game::switchButton() // смена кнопки
{
    for (int i=0; i<numButton; i++)
        menuButtons[i]->deselect();

    menuButtons[curButton]->select();
}
void Game::pve()
{
    started = true;
    scene->clear();

    // фон карты
    setBackgroundBrush(QBrush(QColor(230,230,230,255)));

    // создание игрока
    player = new Player();
    scene->addItem(player);
    scene->addItem(player->head);

    // очки
    score = new Score();
    scene->addItem(score);

    // здоровье
    /*
    health = new Health();
    health->setPos(player->x()+40,player->y()+50);
    scene->addItem(health);
    */

    // враги
    QTimer * timer = new QTimer();
    QObject::connect(timer,SIGNAL(timeout()),player,SLOT(spawn()));
    timer->start(2000);


    // фоновая музыка
    /*
    QMediaPlayer * music = new QMediaPlayer();
    music->setMedia(QUrl("qrc:/sounds/sounds/hellmarch.mp3"));
    music->play();
    */
}

void Game::pvp()
{
    scene->clear();

    // фон карты
    setBackgroundBrush(QBrush(QColor(230,230,230,255)));

    // файл открываем
    QString path = QDir::currentPath() + "/maps";
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), path, tr("Map (*.map)")); // только .map

    // проверка
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);

    //берем первую строку
    QString line = in.readLine();
    QStringList list1;
    list1 = line.split(" ");
    // размеры карты
    yBlocks = list1[0].toInt();
    xBlocks = list1[1].toInt();
    // кол-во точек спавна
    spawns = list1[2].toInt();

    //qDebug() << yBlocks << " " << xBlocks << " " << spawns;

    int sum = (xBlocks+1)*(yBlocks+1);
    Block **pixBlocks=new Block *[sum];

    QString img;
    int width = 60; // размер блоков
    int height = 60;
    int x,y;

    scene->setSceneRect(0,0,xBlocks*width,yBlocks*height); // разрешение сцены
    setFixedSize(xBlocks*width,yBlocks*height);

    for (int i = 0; i < yBlocks; i++)
    {
        line = in.readLine();
        list1 = line.split(" ");
        for (int j = 0; j < xBlocks; j++)
        {
            if (list1[j] != "S" && list1[j] != "0" && list1[j] != "")
            {
                x = width * j;
                y = height * i;
                img = ":/images/images/blocks/" + list1[j] + ".png";
                pixBlocks[i+j] = new Block();
                pixBlocks[i+j]->setPixmap(QPixmap(img).scaled(width,height));
                pixBlocks[i+j]->setPos(x,y);
                scene->addItem(pixBlocks[i+j]);
            }

        }
    }
    file.close();

    // создание игрока
    player = new Player();
    scene->addItem(player);
    scene->addItem(player->head);

    // очки
    score = new Score();
    scene->addItem(score);



    // заглушка
    /*
    scene->clear();
    QMediaPlayer *voice = new QMediaPlayer();
    voice->setMedia(QUrl("qrc:/sounds/sounds/notsurprised.wav"));
    voice->play();
    delay(1000);
    menu();
*/
}

void Game::settings()
{
    scene->clear();
    int random = 1 + rand()%2;
    QMediaPlayer *voice = new QMediaPlayer();
    voice->setMedia(QUrl("qrc:/sounds/sounds/no" + QString::number(random) + ".wav"));
    voice->play();
    delay(1000);
    menu();
}

void Game::menu()
{
    scene->clear();
    setBackgroundBrush(QBrush(QColor(230,230,230,255)));

    curButton = 0;
    pressed = false;

    // надпись
    QGraphicsTextItem *title = new QGraphicsTextItem(QString("TANKBATTLES"));
    title->setDefaultTextColor(QColor(71, 71, 71, 255));
    QFont titleFont("Century Gothic",70);
    title->setFont(titleFont);
    int txPos = this->width()/2 - title->boundingRect().width()/2;
    int tyPos = 50;
    title->setPos(txPos,tyPos);
    scene->addItem(title);


    // кнопки
    int xPos;
    int yPos = 200; // y координата верхней кнопки
    QString text[numButton] = {"PvE","PvP","Settings","Exit"};

    for (int i=0; i<numButton; i++)
    {
        menuButtons[i] = new Button(i,text[i],275,70);
        xPos = this->width()/2 - menuButtons[i]->boundingRect().width()/2;
        menuButtons[i]->setPos(xPos,yPos);
        scene->addItem(menuButtons[i]);
        yPos += 80; // для следующей кнопки
    }
    // действия кнопок
    connect(menuButtons[0],SIGNAL(clicked()),this,SLOT(pve()));
    connect(menuButtons[1],SIGNAL(clicked()),this,SLOT(pvp()));
    connect(menuButtons[2],SIGNAL(clicked()),this,SLOT(settings()));
    connect(menuButtons[3],SIGNAL(clicked()),this,SLOT(close()));

    switchButton();
}

void Game::delay( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}
















