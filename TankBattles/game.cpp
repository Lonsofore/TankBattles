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
#include <QDesktopWidget>
#include "numupdown.h"
#include "delay.h"
#include <QMediaPlaylist>
#include <QtNetwork>

bool started = false;
bool inmenu = false;
int curButton;

Game::Game(QWidget *parent){
    // размеры окна по умолчанию
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

    // всего кнопок в меню 4
    pressed = false;

    // кол-во спавнов на карте
    spawns = 0;

    // открываем файл настроек
    QFile file("settings.cfg");
    file.open(QIODevice::ReadWrite);
    QTextStream in(&file);

    // разрешение экрана
    maxwidth = 800;
    maxheight = 600;
    QString line = in.readLine();
    //qDebug() << line;
    if (line != "")
    {
        QStringList list;
        list = line.split("x");
        int width = list[0].toInt();
        int height = list[1].toInt();
        if (width >= 640 && height >= 480)
        {
            maxheight = height;
            maxwidth = width;
        }
    }
    else
    {
        in << maxwidth << "x" << maxheight << endl;
    }
    setFixedSize(maxwidth,maxheight);
    scene->setSceneRect(0,0,maxwidth,maxheight); // разрешение сцены
    moveToCenter(); // окно по центру

    // громкость музыки
    vmusic = 50;
    line = in.readLine();
    if (line != "")
    {
        if (line.toInt() >= 0 && line.toInt() <= 100)
            vmusic = line.toInt();
    }
    else
    {
        in << vmusic << endl;
    }

    // громкость эффектов
    veffects = 100;
    line = in.readLine();
    if (line != "")
    {
        if (line.toInt() >= 0 && line.toInt() <= 100)
            veffects = line.toInt();
    }
    else
    {
        in << veffects << endl;
    }

    file.close();

    show();
}
//Прием данных

void Game::mouseReleaseEvent(QMouseEvent *event)
{
    if (started)
        player->setFocus();
    if (inmenu)
        menuButtons[curButton]->setFocus();
}

void Game::focusOutEvent(QFocusEvent *event)
{
    if (started)
        player->playerReset();
}

void Game::wheelEvent(QWheelEvent * event)
{
    if (started)
        centerOn(player);
}

void Game::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::WindowStateChange && started == true)
    {
        player->playerReset();
    }
}

void Game::switchButton(int n) // смена кнопки
{
    if (n>=0)
    {
        if (n < numButtons)
            curButton = n;
        else
            curButton = 0;
    }
    else
        curButton = numButtons - 1;

    for (int i=0; i<numButtons; i++)
        menuButtons[i]->deselect();

    menuButtons[curButton]->select();
}

// перемещает окно в центр экрана
void Game::moveToCenter()
{
    QDesktopWidget desktop;
    QRect rect = desktop.availableGeometry(desktop.primaryScreen()); // прямоугольник с размерами экрана
    QPoint center = rect.center(); //координаты центра экрана
    center.setX(center.x() - (this->width()/2));  // учитываем половину ширины окна
    center.setY(center.y() - (this->height()/2));  // .. половину высоты
    move(center);
}

void Game::pve()
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
    {
        menu();
        return;
    }

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
    spawnPoints=new QPoint*[spawns];

    //qDebug() << yBlocks << " " << xBlocks << " " << spawns;

    QString img;
    int width = 60; // размер блоков
    int height = 60;
    int x,y;

    scene->setSceneRect(0,0,xBlocks*width,yBlocks*height); // разрешение сцены
    if (xBlocks*width <= maxwidth && yBlocks*height <= maxheight) // если разрешение, выставленное игроком, больше того, что на карте
        setFixedSize(xBlocks*width,yBlocks*height);
    moveToCenter();

    int num = 0;
    for (int i = 0; i < yBlocks; i++)
    {
        line = in.readLine();
        list1 = line.split(" ");
        for (int j = 0; j < xBlocks; j++)
        {
            if (list1[j] != "S" && list1[j] != "0" && list1[j] != "")
            {
                Block *block = new Block(list1[j].toInt());
                x = width * j;
                y = height * i;
                img = ":/images/images/blocks/" + list1[j] + ".png";
                block->setPixmap(QPixmap(img).scaled(width,height));
                block->setPos(x,y);
                scene->addItem(block);
            }

            if (list1[j] == "S")
            {
                // создание массива точек для спавна
                spawnPoints[num] = new QPoint(width*j,height*i);
                num++;
            }
        }
    }
    file.close();

    // создание игрока
    //Tank * enmy;
    enmy = new Tank();
    scene->addItem(enmy);
    scene->addItem(enmy->head);
    enmy->ChangePosition(10, 60);
    player = new Player();
    scene->addItem(player);
    scene->addItem(player->head);

    // очки
    score = new Score();
    scene->addItem(score);

    // музыка
    QMediaPlayer * music = new QMediaPlayer();
    QMediaPlaylist * playlist = new QMediaPlaylist();
    playlist->addMedia(QUrl("qrc:/sounds/sounds/ambient.mp3"));
    //playlist->setPlaybackMode(QMediaPlaylist::PlaybackMode::Loop);
    music->setPlaylist(playlist);
    //music->setMedia(QUrl("qrc:/sounds/sounds/ambient.mp3"));
    music->setVolume(vmusic); // уровень громкости (из 100)
    music->play();

    started = true;
    inmenu = false;
    //Подготовка к приему данных
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(45454, QUdpSocket::ShareAddress);
    connect(udpSocket, SIGNAL(readyRead()),
            this, SLOT(processPendingDatagrams()));
}

void Game::pvp()
{
    // заглушка
    scene->clear();
    QMediaPlayer *voice = new QMediaPlayer();
    voice->setMedia(QUrl("qrc:/sounds/sounds/notsurprised.wav"));
    voice->play();
    delay(1000);
    menu();
}

void Game::settings()
{
    scene->clear();
    setBackgroundBrush(QBrush(QColor(230,230,230,255)));
    //delete [] menuButtons;

    inmenu = false;
    pressed = false;

    // надпись
    /*
    QGraphicsTextItem *title = new QGraphicsTextItem(QString("SETTINGS"));
    title->setDefaultTextColor(QColor(71, 71, 71, 255));
    QFont titleFont("Century Gothic",70);
    title->setFont(titleFont);
    int txPos = this->width()/2 - title->boundingRect().width()/2;
    int tyPos = 50;
    title->setPos(txPos,tyPos);
    scene->addItem(title);
    */

    // точки
    int xPos;
    int yPos = 120; //верхняя точка

    QString resArray[11] = {"640x480",
                            "800x480",
                            "800x600",
                            "960x540",
                            "1024x768",
                            "1280x720",
                            "1366x768",
                            "1440x900",
                            "1600x900",
                            "1680x1050",
                            "1920x1080",};

    int n;
    for (int i=0; i<11; i++) // вычисление текущего разрешения
    {
        QStringList list;
        list = resArray[i].split("x");
        int width = list[0].toInt();
        int height = list[1].toInt();
        if (width == this->width() && height == this->height())
            n = i;
    }

    int n1,n2;
    QString arr[11] = {"0","10","20","30","40","50","60","70","80","90","100"};
    for (int i=0; i<11; i++)
    {
        if (arr[i].toInt() == vmusic)
            n1 = i;
        if (arr[i].toInt() == veffects)
            n2 = i;
    }

    res = new numUpDown(resArray, n, 300, 70);
    xPos = this->width()/2 - res->boundingRect().width()/2;
    res->setPos(xPos,yPos);
    scene->addItem(res);

    text1 = new TextPanel("Music", 180, 70);
    yPos += 80;
    text1->setPos(xPos,yPos);
    scene->addItem(text1);

    int xPos1 = xPos + text1->boundingRect().width();
    num1 = new numUpDown(arr, n1, 120, 70);
    num1->setPos(xPos1,yPos);
    scene->addItem(num1);

    text2 = new TextPanel("Sound", 180, 70);
    yPos += 80;
    text2->setPos(xPos,yPos);
    scene->addItem(text2);

    num2 = new numUpDown(arr, n2, 120, 70);
    num2->setPos(xPos1,yPos);
    scene->addItem(num2);

    yPos += 80;
    apply = new Button(0, "Apply", 300, 70);
    apply->setPos(xPos,yPos);
    scene->addItem(apply);
    connect(apply,SIGNAL(clicked()),this,SLOT(applySettings()));

    yPos += 80;
    back = new Button(1, "Back", 300, 70);
    back->setPos(xPos,yPos);
    scene->addItem(back);
    connect(back,SIGNAL(clicked()),this,SLOT(menu()));

    // заглушка
    /*
    scene->clear();
    int random = 1 + rand()%2;
    QMediaPlayer *voice = new QMediaPlayer();
    voice->setMedia(QUrl("qrc:/sounds/sounds/no" + QString::number(random) + ".wav"));
    voice->play();
    delay(1000);
    menu();
    */
}

void Game::applySettings()
{
    QString text = res->text->toPlainText();
    QStringList list;
    list = text.split("x");
    int width = list[0].toInt();
    int height = list[1].toInt();

    // если юзер изменил разрешение
    if (width != maxwidth && height != maxheight)
    {
        setFixedSize(width,height);
        scene->setSceneRect(0,0,width,height); // разрешение сцены
        moveToCenter(); // окно по центру

        maxwidth = this->width();
        maxheight = this->height();
    }

    vmusic = num1->text->toPlainText().toInt();
    veffects = num2->text->toPlainText().toInt();

    // удаляем прошлые настройки
    QFile("settings.cfg").remove();

    // открываем файл настроек
    QFile file("settings.cfg");
    file.open(QIODevice::WriteOnly);
    QTextStream in(&file);

    // записываем новые настройки
    in << maxwidth << "x" << maxheight << endl;
    in << vmusic << endl;
    in << veffects << endl;

    //возврат в настройки
    settings();
}


void Game::menu()
{
    started = false;
    inmenu = true;

    scene->clear();
    setBackgroundBrush(QBrush(QColor(230,230,230,255)));

    // максимальные поддерживаемые размеры карты. если больше - будет обрезаться
    //maxwidth = this->width();
    //maxheight = this->height();

    // кол-во кнопок
    numButtons = 4;
    // не нажата
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

    // названия для кнопок
    QString *text;
    text = new QString[numButtons];
    text[0] = "PvE";
    text[1] = "PvP";
    text[2] = "SETTINGS";
    text[3] = "EXIT";

    menuButtons = new Button*[numButtons];

    for (int i=0; i<numButtons; i++)
    {
        menuButtons[i] = new Button(i, text[i], 275, 70);
        xPos = this->width()/2 - menuButtons[i]->boundingRect().width()/2;
        menuButtons[i]->setPos(xPos,yPos);
        scene->addItem(menuButtons[i]);
        connect(menuButtons[i],SIGNAL(changed(int)),this,SLOT(switchButton(int)));
        yPos += 80; // для следующей кнопки
    }
    // действия кнопок
    connect(menuButtons[0],SIGNAL(clicked()),this,SLOT(pve()));
    connect(menuButtons[1],SIGNAL(clicked()),this,SLOT(pvp()));
    connect(menuButtons[2],SIGNAL(clicked()),this,SLOT(settings()));
    connect(menuButtons[3],SIGNAL(clicked()),this,SLOT(close()));

    //curButton = 0;
    switchButton(0); // по умолчанию выбрать первую кнопку
}

void Game::processPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        QString xval, yval; //Говнокод!!1111
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
        QList<QByteArray> list;
        list = datagram.split(' ');
        //xval = datagram.at(1);
        //yval = datagram.at(3);
        enmy->ChangePosition(list.at(0).toInt(),list.at(1).toInt());
    }
}







