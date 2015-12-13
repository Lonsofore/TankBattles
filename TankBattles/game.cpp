#include "game.h"
#include <QTimer>
#include <QGraphicsTextItem>
#include <QFont>
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
#include <QFontDatabase>
#include <QtNetwork>

// где находится игрок
bool inpve = false;   // пве
bool inpvp = false;   // пвп
bool inpvp1 = false;  // присоединиться к игре
bool inpvp2 = false;  // создать игру
bool insett = false;  // общие настройки
bool insett1 = false; // настройки игрока
bool insett2 = false; // настройки игры
bool inmenu = false;  // меню
bool ingmenu = false; // внутриигровое меню

int from;

const int nMenuBtns = 4;
const int nSettBtns = 3;
const int nSett1Btns = 2;
const int nSett2Btns = 2;
const int nSett2UpDn = 3;
const int nPvpBtns = 3;
const int nGMenuBtns = 4;

int curButton;

static inline QByteArray IntToArray(qint32 source);

Game::Game(QWidget *parent)
{
    // размеры окна по умолчанию
    int width = 800;
    int height = 600;

    // создание сцены
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,width,height); // разрешение сцены
    setScene(scene);

    scene->setStickyFocus(true);

    // чтобы не появлялись скроллбары
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(width,height); // разрешение экрана

    // кол-во спавнов на карте
    spawns = 0;
    blockSize = 70;

    // открываем файл настроек
    QFile file("settings.cfg");
    file.open(QIODevice::ReadWrite);
    QTextStream in(&file);

    // разрешение экрана
    maxwidth = 800;
    maxheight = 600;
    QString line = in.readLine();
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

    numtank = 0;

    show();
}

void Game::change(QString name)
{
    inmenu = false;
    inpve = false;
    inpvp = false;
    inpvp1 = false;
    inpvp2 = false;
    insett = false;
    insett1 = false;
    insett2 = false;
    ingmenu = false;

    if (name == "menu")
    {
        inmenu = true;
        return;
    }

    if (name == "pve")
    {
        inpve = true;
        return;
    }

    if (name == "pvp")
    {
        inpvp = true;
        return;
    }

    if (name == "pvp1")
    {
        inpvp1 = true;
        return;
    }

    if (name == "pvp2")
    {
        inpvp2 = true;
        return;
    }

    if (name == "sett")
    {
        insett = true;
        return;
    }

    if (name == "sett1")
    {
        insett1 = true;
        return;
    }

    if (name == "sett2")
    {
        insett2 = true;
        return;
    }

    if (name == "gmenu")
    {
        ingmenu = true;
        return;
    }
}

void Game::switchButton(int n) // смена кнопки
{
    if (inmenu) // главное меню
    {
        if (n < 0)
            n = n + nMenuBtns;
        if (n > nMenuBtns - 1)
            n = n - nMenuBtns;

        curButton = n;

        for (int i=0; i<nMenuBtns; i++)
            btns[i]->deselect();

        btns[curButton]->select();
    }

    if (inpve) // пве
    {

    }

    if (inpvp) // пвп
    {
        if (n < 0)
            n = n + nPvpBtns;
        if (n > nPvpBtns - 1)
            n = n - nPvpBtns;

        curButton = n;

        for (int i=0; i<nPvpBtns; i++)
            btns[i]->deselect();

        btns[curButton]->select();
    }

    if (inpvp1) // присоединиться к игре
    {

    }

    if (inpvp2) // создать игру
    {

    }

    if (insett) // список настроек
    {
        if (n < 0)
            n = n + nSettBtns;
        if (n > nSettBtns - 1)
            n = n - nSettBtns;

        curButton = n;

        for (int i=0; i<nSettBtns; i++)
            btns[i]->deselect();

        btns[curButton]->select();
    }

    if (insett1) // настройки игрока
    {

    }

    if (insett2) // основные настройки
    {
        if (n < 0)
            n = n + nSett2Btns + nSett2UpDn;
        if (n > nSett2Btns + nSett2UpDn - 1)
            n = n - nSett2Btns - nSett2UpDn;

        curButton = n;

        for (int i=0; i<nSett2Btns; i++)
            btns[i]->deselect();

        for (int i=0; i<nSett2UpDn; i++)
            udBtns[i]->deselect();

        switch (curButton)
        {
            case 0:
                udBtns[0]->select();
            break;

            case 1:
                udBtns[1]->select();
            break;

            case 2:
                udBtns[2]->select();
            break;

            case 3:
                btns[0]->select();
            break;

            case 4:
                btns[1]->select();
            break;
        }
    }

    if (ingmenu) // внутриигровое меню
    {
        if (n < 0)
            n = n + nGMenuBtns;
        if (n > nGMenuBtns - 1)
            n = n - nGMenuBtns;

        curButton = n;

        for (int i=0; i<nGMenuBtns; i++)
            btns[i]->deselect();

        btns[curButton]->select();
    }
}

void Game::menu()
{
    change("menu");

    scene->clear();
    setBackgroundBrush(QBrush(QColor(230,230,230,255)));

    // шрифт
    QFontDatabase::addApplicationFont(":/fonts/fonts/GOTHIC.TTF");

    // надпись
    QGraphicsTextItem *title = new QGraphicsTextItem(QString("TANKBATTLES"));
    title->setDefaultTextColor(QColor(71, 71, 71, 255));
    QFont titleFont("Century Gothic",70);
    title->setFont(titleFont);
    int txPos = this->width()/2 - title->boundingRect().width()/2;
    int tyPos = this->height()/2 - 270;
    title->setPos(txPos,tyPos);
    scene->addItem(title);

    // кнопки
    int xPos;
    int yPos = tyPos + 150; // y координата верхней кнопки

    // названия для кнопок
    QString *text;
    text = new QString[4];
    text[0] = "PvE";
    text[1] = "PvP";
    text[2] = "SETTINGS";
    text[3] = "EXIT";

    btns = new Button*[text->size()+1];

    for (int i=0; i<text->size()+1; i++)
    {
        btns[i] = new Button(i, text[i], 275, 70);
        xPos = this->width()/2 - btns[i]->boundingRect().width()/2;
        btns[i]->setPos(xPos,yPos);
        scene->addItem(btns[i]);
        connect(btns[i],SIGNAL(changed(int)),this,SLOT(switchButton(int)));
        yPos += 80; // для следующей кнопки
    }
    // действия кнопок
    connect(btns[0],SIGNAL(clicked()),this,SLOT(pve()));
    connect(btns[1],SIGNAL(clicked()),this,SLOT(pvp()));
    connect(btns[2],SIGNAL(clicked()),this,SLOT(settings()));
    connect(btns[3],SIGNAL(clicked()),this,SLOT(close()));

    switchButton(0); // по умолчанию выбрать первую кнопку
}

void Game::pve()
{
    scene->clear();

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

    // фон карты
    setBackgroundBrush(QBrush(QColor(229,229,229,255)));

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

    QString img;
    int width = blockSize; // размер блоков
    int height = blockSize;
    int x,y;

    dop = 2*width; // дополнительное кол-во блоков за картой
    scene->setSceneRect(0,0,xBlocks*width+dop*2,yBlocks*height+dop*2); // разрешение сцены

    // если разрешение, выставленное игроком, больше того, что на карте
    if (xBlocks*width <= maxwidth) // по ширине
        setFixedWidth(xBlocks*width);
    if (yBlocks*height <= maxheight) // по высоте
        setFixedHeight(yBlocks*height);

    moveToCenter();

    // создание линий ограничения
    int pwidth = 10; // ширина линии
    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setColor(QColor(179, 179, 179, 255));
    pen.setJoinStyle(Qt::MiterJoin);
    pen.setWidth(pwidth);

    int pp = pwidth + 4; // чтобы линии были не вплотную к блокам
    // чтобы понятнее было:
    // p1 p2
    // p4 p3
    QPointF p1(dop-pp, dop-pp);
    QPointF p2(xBlocks*width+dop+pp, dop-pp);
    QPointF p3(xBlocks*width+dop+pp, yBlocks*height+dop+pp);
    QPointF p4(dop-pp, yBlocks*height+dop+pp);
    scene->addLine(QLineF(p1,p2),pen);
    scene->addLine(QLineF(p2,p3),pen);
    scene->addLine(QLineF(p3,p4),pen);
    scene->addLine(QLineF(p4,p1),pen);

    // считывание карты из файла
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
                x = width * j + dop;
                y = height * i + dop;
                img = ":/images/images/blocks/" + list1[j] + ".png";
                block->setPixmap(QPixmap(img).scaled(width,height));
                block->setPos(x,y);
                scene->addItem(block);
            }

            if (list1[j] == "S")
            {
                // создание массива точек для спавна
                spawnPoints[num] = new QPoint(width*j+dop,height*i+dop);
                num++;
            }
        }
    }
    file.close();

    // тестовый танк
    enmy = new Tank();
    scene->addItem(enmy);
    scene->addItem(enmy->head);
    enmy->changePos(200,100);

    // создание игрока
    player = new Player();
    scene->addItem(player);
    scene->addItem(player->head);
    connect(player,SIGNAL(tomenu()),this,SLOT(gameMenu()));

    connect(player, SIGNAL(KeyPressed()), this, SLOT(SendData()));


    // очки
    //score = new Score();
    //scene->addItem(score);

    // музыка
    QMediaPlayer * music = new QMediaPlayer();
    QMediaPlaylist * playlist = new QMediaPlaylist();
    playlist->addMedia(QUrl("qrc:/sounds/sounds/ambient.mp3"));
    //playlist->setPlaybackMode(QMediaPlaylist::PlaybackMode::Loop);
    music->setPlaylist(playlist);
    //music->setMedia(QUrl("qrc:/sounds/sounds/ambient.mp3"));
    music->setVolume(vmusic); // уровень громкости (из 100)
    music->play();

    change("pve");

    //Подготовка к приему данных
    tcpSocket = new QTcpSocket();
    tcpSocket->connectToHost("192.168.0.112", 7);
    tcpSocket->waitForConnected(3000000);
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(45454, QUdpSocket::ShareAddress);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
}

void Game::pvp()
{
    scene->clear();
    setBackgroundBrush(QBrush(QColor(229,229,229,255)));

    change("pvp");

    // точки
    int xPos;
    int yPos = this->height()/2 - 270; //верхняя точка

    // надпись
    QGraphicsTextItem *title = new QGraphicsTextItem(QString("PvP"));
    title->setDefaultTextColor(QColor(71, 71, 71, 255));
    QFont titleFont("Century Gothic",70);
    title->setFont(titleFont);
    xPos = this->width()/2 - title->boundingRect().width()/2;
    title->setPos(xPos,yPos);
    scene->addItem(title);

    // обычные кнопки
    btns = new Button*[nPvpBtns];

    // присоединиться к игре
    yPos += 150;
    btns[0] = new Button(0, "Join", 300, 70);
    xPos = this->width()/2 - btns[0]->boundingRect().width()/2;
    btns[0]->setPos(xPos,yPos);
    scene->addItem(btns[0]);
    connect(btns[0],SIGNAL(clicked()),this,SLOT(pvp1()));
    connect(btns[0],SIGNAL(changed(int)),this,SLOT(switchButton(int)));
    connect(btns[0],SIGNAL(back()),this,SLOT(menu()));

    // создать игру
    yPos += 80;
    btns[1] = new Button(1, "Create", 300, 70);
    btns[1]->setPos(xPos,yPos);
    scene->addItem(btns[1]);
    connect(btns[1],SIGNAL(clicked()),this,SLOT(pvp2()));
    connect(btns[1],SIGNAL(changed(int)),this,SLOT(switchButton(int)));
    connect(btns[1],SIGNAL(back()),this,SLOT(menu()));

    // вернуться назад
    yPos += 80;
    btns[2] = new Button(2, "Back", 300, 70);
    btns[2]->setPos(xPos,yPos);
    scene->addItem(btns[2]);
    connect(btns[2],SIGNAL(clicked()),this,SLOT(menu()));
    connect(btns[2],SIGNAL(changed(int)),this,SLOT(switchButton(int)));
    connect(btns[2],SIGNAL(back()),this,SLOT(menu()));

    switchButton(0); // по умолчанию выбрать первую кнопку
}

void Game::pvp1()
{
    change("pvp1");

    usrid = 1;
    pve();

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

void Game::pvp2()
{
    change("pvp2");

    usrid = 0;
    pve();

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
    setBackgroundBrush(QBrush(QColor(229,229,229,255)));

    change("sett");

    // точки
    int xPos;
    int yPos = this->height()/2 - 270; //верхняя точка

    // надпись
    QGraphicsTextItem *title = new QGraphicsTextItem(QString("SETTINGS"));
    title->setDefaultTextColor(QColor(71, 71, 71, 255));
    QFont titleFont("Century Gothic",70);
    title->setFont(titleFont);
    xPos = this->width()/2 - title->boundingRect().width()/2;
    title->setPos(xPos,yPos);
    scene->addItem(title);

    // обычные кнопки
    btns = new Button*[nSettBtns];

    // настройки игрока
    yPos += 150;
    btns[0] = new Button(0, "Player", 300, 70);
    xPos = this->width()/2 - btns[0]->boundingRect().width()/2;
    btns[0]->setPos(xPos,yPos);
    scene->addItem(btns[0]);
    connect(btns[0],SIGNAL(clicked()),this,SLOT(pSettings()));
    connect(btns[0],SIGNAL(changed(int)),this,SLOT(switchButton(int)));
    connect(btns[0],SIGNAL(back()),this,SLOT(menu()));

    // настройки игры
    yPos += 80;
    btns[1] = new Button(1, "General", 300, 70);
    btns[1]->setPos(xPos,yPos);
    scene->addItem(btns[1]);
    connect(btns[1],SIGNAL(clicked()),this,SLOT(mSettings()));
    connect(btns[1],SIGNAL(changed(int)),this,SLOT(switchButton(int)));
    connect(btns[1],SIGNAL(back()),this,SLOT(menu()));

    // вернуться назад
    yPos += 80;
    btns[2] = new Button(2, "Back", 300, 70);
    btns[2]->setPos(xPos,yPos);
    scene->addItem(btns[2]);
    connect(btns[2],SIGNAL(clicked()),this,SLOT(menu()));
    connect(btns[2],SIGNAL(changed(int)),this,SLOT(switchButton(int)));
    connect(btns[2],SIGNAL(back()),this,SLOT(menu()));

    switchButton(0); // по умолчанию выбрать первую кнопку
}

void Game::pSettings()
{
    scene->clear();
    setBackgroundBrush(QBrush(QColor(229,229,229,255)));

    change("sett1");

    // заглушка

    scene->clear();
    int random = 1 + rand()%2;
    QMediaPlayer *voice = new QMediaPlayer();
    voice->setMedia(QUrl("qrc:/sounds/sounds/no" + QString::number(random) + ".wav"));
    voice->play();
    delay(1000);
    settings();

}

void Game::mSettings()
{
    scene->clear();
    setBackgroundBrush(QBrush(QColor(229,229,229,255)));

    change("sett2");
    from = 0;

    // точки
    int xPos;
    int yPos = this->height()/2 - 195; //верхняя точка

    const int resnum = 9;
    QString resArray[resnum] = {"800x600",
                                "960x540",
                                "1024x768",
                                "1280x720",
                                "1366x768",
                                "1440x900",
                                "1600x900",
                                "1680x1050",
                                "1920x1080",};

    int n;
    for (int i=0; i<resnum; i++) // вычисление текущего разрешения
    {
        QStringList list;
        list = resArray[i].split("x");
        int width = list[0].toInt();
        int height = list[1].toInt();
        if (width == this->width() && height == this->height())
            n = i;
    }

    int n1,n2;
    const int arrnum = 11;
    QString arr[arrnum] = {"0","10","20","30","40","50","60","70","80","90","100"};
    for (int i=0; i<arrnum; i++)
    {
        if (arr[i].toInt() == vmusic)
            n1 = i;
        if (arr[i].toInt() == veffects)
            n2 = i;
    }

    // обычные кнопки и кнопки-счетчики
    btns = new Button*[nSett2Btns];
    udBtns = new numUpDown*[nSett2UpDn];

    // разрешение экрана
    udBtns[0] = new numUpDown(0, resArray, resnum, n, 300, 70);
    xPos = this->width()/2 - udBtns[0]->boundingRect().width()/2;
    udBtns[0]->setPos(xPos,yPos);
    scene->addItem(udBtns[0]);
    connect(udBtns[0],SIGNAL(changed(int)),this,SLOT(switchButton(int)));

    QString img = ":/images/images/menu/Panel.png";

    // плашка музыка
    text1 = new TextPanel("Music", img, 180, 70);
    yPos += 80;
    text1->setPos(xPos,yPos);
    scene->addItem(text1);

    // громкость музыки
    int xPos1 = xPos + text1->boundingRect().width();
    udBtns[1] = new numUpDown(1, arr, arrnum, n1, 120, 70);
    udBtns[1]->setPos(xPos1,yPos);
    scene->addItem(udBtns[1]);
    connect(udBtns[1],SIGNAL(changed(int)),this,SLOT(switchButton(int)));

    // плашка звук
    text2 = new TextPanel("Sound", img, 180, 70);
    yPos += 80;
    text2->setPos(xPos,yPos);
    scene->addItem(text2);

    // громкость звуков
    udBtns[2] = new numUpDown(2, arr, arrnum, n2, 120, 70);
    udBtns[2]->setPos(xPos1,yPos);
    scene->addItem(udBtns[2]);
    connect(udBtns[2],SIGNAL(changed(int)),this,SLOT(switchButton(int)));

    for (int i=0; i < nSett2UpDn; i++)
    {
        connect(udBtns[i],SIGNAL(back()),this,SLOT(settings()));
    }

    // применить
    yPos += 80;
    btns[0] = new Button(3, "Apply", 300, 70);
    btns[0]->setPos(xPos,yPos);
    scene->addItem(btns[0]);
    connect(btns[0],SIGNAL(clicked()),this,SLOT(applySettings()));
    connect(btns[0],SIGNAL(changed(int)),this,SLOT(switchButton(int)));

    // вернуться назад
    yPos += 80;
    btns[1] = new Button(4, "Back", 300, 70);
    btns[1]->setPos(xPos,yPos);
    scene->addItem(btns[1]);
    connect(btns[1],SIGNAL(clicked()),this,SLOT(settings()));
    connect(btns[1],SIGNAL(changed(int)),this,SLOT(switchButton(int)));

    for (int i=0; i < nSett2Btns; i++)
    {
        connect(btns[i],SIGNAL(back()),this,SLOT(settings()));
    }

    switchButton(0); // по умолчанию выбрать первую кнопку

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
    QString text = udBtns[0]->text->toPlainText();
    QStringList list;
    list = text.split("x");
    int width = list[0].toInt();
    int height = list[1].toInt();

    // если юзер изменил разрешение
    if (width != maxwidth || height != maxheight)
    {
        if (from == 0)
        {
            scene->setSceneRect(0,0,width,height); // разрешение сцены
        }
        else
        {
            if (xBlocks*blockSize <= width) // по ширине
                setFixedWidth(xBlocks*blockSize);
            if (yBlocks*blockSize <= height) // по высоте
                setFixedHeight(yBlocks*blockSize);
        }

        setFixedSize(width,height);
        moveToCenter(); // окно по центру

        maxwidth = this->width();
        maxheight = this->height();
    }

    vmusic = udBtns[1]->text->toPlainText().toInt();
    veffects = udBtns[2]->text->toPlainText().toInt();

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
    if (from == 0)
        mSettings();
    else
    {
        delete backgr;
        delete backgr1;
        delete text1;
        delete text2;
        for (int i = 0; i<nSett2Btns; i++)
        {
            delete btns[i];
        }
        for (int i = 0; i<nSett2UpDn; i++)
        {
            delete udBtns[i];
        }

        centerOn(player);
        gSettings();
    }
}

void Game::toGameMenu()
{
    delete backgr;
    delete backgr1;
    delete text1;
    delete text2;
    for (int i = 0; i<nSett2Btns; i++)
    {
        delete btns[i];
    }
    for (int i = 0; i<nSett2UpDn; i++)
    {
        delete udBtns[i];
    }
    gameMenu();
}

void Game::gameMenu()
{
    change("gmenu");

    // шрифт
    QFontDatabase::addApplicationFont(":/fonts/fonts/GOTHIC.TTF");

    // координата Х в центре экрана
    int x1 = player->centralX();

    // координата Y в центре экрана
    int y1 = player->centralY();

    // кнопки
    int xPos = x1 - 275/2;
    int yPos = y1 - 140; // y координата верхней кнопки

    // фон
    QString img = ":/images/images/menu/Panel.png";
    backgr = new TextPanel("", img, width(), height());
    backgr->setPos(x1-width()/2,y1-height()/2);
    backgr->setZValue(95);
    //scene->addItem(backgr);

    // плашка
    img = ":/images/images/menu/Back.png";
    backgr1 = new TextPanel("", img, 305, 340);
    backgr1->setPos(xPos-15,yPos-15);
    backgr1->setZValue(96);
    scene->addItem(backgr1);

    // названия для кнопок
    QString *text;
    text = new QString[nGMenuBtns];
    text[0] = "Back";
    text[1] = "Settings";
    text[2] = "Menu";
    text[3] = "Exit";

    btns = new Button*[nGMenuBtns];

    for (int i=0; i<nGMenuBtns; i++)
    {
        btns[i] = new Button(i, text[i], 275, 70);
        btns[i]->setPos(xPos,yPos);
        scene->addItem(btns[i]);
        btns[i]->setZValue(100);
        connect(btns[i],SIGNAL(changed(int)),this,SLOT(switchButton(int)));
        connect(btns[i],SIGNAL(back()),this,SLOT(back()));
        yPos += 80; // для следующей кнопки
    }
    // действия кнопок
    connect(btns[0],SIGNAL(clicked()),this,SLOT(back()));
    connect(btns[1],SIGNAL(clicked()),this,SLOT(toSettings()));
    connect(btns[2],SIGNAL(clicked()),this,SLOT(toMenu()));
    connect(btns[3],SIGNAL(clicked()),this,SLOT(close()));

    switchButton(0); // по умолчанию выбрать первую кнопку
}

void Game::toSettings()
{
    // переход в настройки из меню
    delete backgr;
    delete backgr1;
    for (int i = 0; i<nGMenuBtns; i++)
    {
        delete btns[i];
    }
    gSettings();
}

void Game::gSettings()
{
    change("sett2");
    from = 1;

    // координата Х в центре экрана
    int x1 = player->centralX();

    // координата Y в центре экрана
    int y1 = player->centralY();

    // точки
    int xPos = x1 - 150;
    int yPos = y1 - 195; //верхняя точка

    // фон
    QString img = ":/images/images/menu/Panel.png";
    backgr = new TextPanel("", img, width(), height());
    backgr->setPos(x1-width()/2,y1-height()/2);
    backgr->setZValue(95);
    //scene->addItem(backgr);

    // плашка
    img = ":/images/images/menu/Back.png";
    backgr1 = new TextPanel("", img, 330, 420);
    backgr1->setPos(xPos-15,yPos-15);
    backgr1->setZValue(96);
    scene->addItem(backgr1);

    const int resnum = 9;
    QString resArray[resnum] = {"800x600",
                                "960x540",
                                "1024x768",
                                "1280x720",
                                "1366x768",
                                "1440x900",
                                "1600x900",
                                "1680x1050",
                                "1920x1080",};

    int n = 0;
    // это почему-то не работает
    /*
    for (int i=0; i<resnum; i++) // вычисление текущего разрешения
    {
        QStringList list;
        list = resArray[i].split("x");
        int width = list[0].toInt();
        int height = list[1].toInt();
        if (width == this->width() && height == this->height())
            n = i;
    } */

    int n1,n2;
    const int arrnum = 11;
    QString arr[arrnum] = {"0","10","20","30","40","50","60","70","80","90","100"};
    for (int i=0; i<arrnum; i++)
    {
        if (arr[i].toInt() == vmusic)
            n1 = i;
        if (arr[i].toInt() == veffects)
            n2 = i;
    }

    // обычные кнопки и кнопки-счетчики
    btns = new Button*[nSett2Btns];
    udBtns = new numUpDown*[nSett2UpDn];

    // разрешение экрана
    udBtns[0] = new numUpDown(0, resArray, resnum, n, 300, 70);
    xPos = x1 - udBtns[0]->boundingRect().width()/2;
    udBtns[0]->setPos(xPos,yPos);
    scene->addItem(udBtns[0]);
    connect(udBtns[0],SIGNAL(changed(int)),this,SLOT(switchButton(int)));

    img = ":/images/images/menu/Panel.png";

    // плашка музыка
    text1 = new TextPanel("Music", img, 180, 70);
    yPos += 80;
    text1->setPos(xPos,yPos);
    text1->setZValue(100);
    scene->addItem(text1);

    // громкость музыки
    int xPos1 = xPos + text1->boundingRect().width();
    udBtns[1] = new numUpDown(1, arr, arrnum, n1, 120, 70);
    udBtns[1]->setPos(xPos1,yPos);
    scene->addItem(udBtns[1]);
    connect(udBtns[1],SIGNAL(changed(int)),this,SLOT(switchButton(int)));

    // плашка звук
    text2 = new TextPanel("Sound", img, 180, 70);
    yPos += 80;
    text2->setPos(xPos,yPos);
    text2->setZValue(100);
    scene->addItem(text2);

    // громкость звуков
    udBtns[2] = new numUpDown(2, arr, arrnum, n2, 120, 70);
    udBtns[2]->setPos(xPos1,yPos);
    scene->addItem(udBtns[2]);
    connect(udBtns[2],SIGNAL(changed(int)),this,SLOT(switchButton(int)));

    for (int i=0; i < nSett2UpDn; i++)
    {
        connect(udBtns[i],SIGNAL(back()),this,SLOT(toGameMenu()));
        udBtns[i]->setZValue(100);
    }

    // применить
    yPos += 80;
    btns[0] = new Button(3, "Apply", 300, 70);
    btns[0]->setPos(xPos,yPos);
    scene->addItem(btns[0]);
    connect(btns[0],SIGNAL(clicked()),this,SLOT(applySettings()));
    connect(btns[0],SIGNAL(changed(int)),this,SLOT(switchButton(int)));

    // вернуться назад
    yPos += 80;
    btns[1] = new Button(4, "Back", 300, 70);
    btns[1]->setPos(xPos,yPos);
    scene->addItem(btns[1]);
    connect(btns[1],SIGNAL(clicked()),this,SLOT(toGameMenu()));
    connect(btns[1],SIGNAL(changed(int)),this,SLOT(switchButton(int)));

    for (int i=0; i < nSett2Btns; i++)
    {
        connect(btns[i],SIGNAL(back()),this,SLOT(toGameMenu()));
        btns[i]->setZValue(100);
    }

    switchButton(0); // по умолчанию выбрать первую кнопку
}

void Game::toMenu()
{
    // тут сделать сигнал, что игрок отключился
    scene->setSceneRect(0,0,maxwidth,maxheight);
    setFixedSize(maxwidth, maxheight);
    menu();
}

void Game::back()
{
    delete backgr;
    delete backgr1;
    for (int i = 0; i<nGMenuBtns; i++)
    {
        delete btns[i];
    }
    player->setFocus();
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

void Game::mouseReleaseEvent(QMouseEvent *event)
{
    if (inpve)
        player->setFocus();
    if (inmenu || insett || insett1 || insett2 || inpvp);
        switchButton(curButton);
}

void Game::focusOutEvent(QFocusEvent *event)
{
    if (inpve)
        player->playerReset();
}

void Game::wheelEvent(QWheelEvent * event)
{
    if (inpve)
        centerOn(player);
}

void Game::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::WindowStateChange && inpve == true)
    {
        player->playerReset();
    }
}

void Game::processPendingDatagrams()
{   int otherplayer; //Временный костыль
    if (usrid == 1) otherplayer = 0;
    if (usrid == 0) otherplayer = 1;
    isrecieving = 1;
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
        QList<QByteArray> playersList;
        playersList = datagram.split('|');
        //Здесь будет цикл
        QString temp;
        temp +=QString::fromLatin1(playersList.at(otherplayer).data());
        QList<QString> dataList;
        dataList =  temp.split(' ');
        enmy->changePos(dataList.at(1).toInt(),dataList.at(2).toInt());
        enmy->changeAngle(dataList.at(3).toInt(), dataList.at(4).toInt());
    }
    isrecieving = 0;
}

void Game::SendData()
{
    //while(isrecieving) {wait();} ищу замену для wait
    if(tcpSocket->state() == QAbstractSocket::ConnectedState)
        {
            QByteArray data = QByteArray::number(usrid) + ' ' + QByteArray::number(player->GetX()) + ' ' + QByteArray::number(player->GetY()) + ' '
                    + QByteArray::number(player->GetTAngle()) + ' ' + QByteArray::number(player->GetHAngle());
            tcpSocket->write(IntToArray(data.size())); //write size of data
            tcpSocket->write(data); //write the data itself
            tcpSocket->waitForBytesWritten();
            return;
        }
        else
            //QMessageBox::information(this, "ERROR", "Connection error");
            return;
}

QByteArray IntToArray(qint32 source)
{
    //Avoid use of cast, this is the Qt way to serialize objects
    QByteArray temp;
    QDataStream data(&temp, QIODevice::ReadWrite);
    data << source;
    return temp;
}
