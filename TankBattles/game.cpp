#include "game.h"
#include <QTimer>
#include <QGraphicsTextItem>
#include <QFont>
#include "enemy.h"
#include <QMediaPlayer>
#include "button.h"
#include "player.h"

bool started = false;

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

    //show();
}

void Game::mouseReleaseEvent(QMouseEvent *event)
{
    if (started)
        player->setFocus();
}

void Game::start()
{
    started = true;
    scene->clear();

    // фон карты
    setBackgroundBrush(QBrush(QColor(230,230,230,255)));

    // создание игрока
    //player = new Tank();
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

void Game::menu()
{
    scene->clear();
    setBackgroundBrush(QBrush(QColor(230,230,230,255)));

    // надпись
    QGraphicsTextItem *title = new QGraphicsTextItem(QString("TankBattles"));
    QFont titleFont("Century Gothic",70);
    title->setFont(titleFont);
    int txPos = this->width()/2 - title->boundingRect().width()/2;
    int tyPos = 50;
    title->setPos(txPos,tyPos);
    scene->addItem(title);


    // кнопки
    int xPos;
    int yPos;
    Button *pveButton = new Button(QString("PvE"),275,70);
    xPos = this->width()/2 - pveButton->boundingRect().width()/2;
    yPos = 200;
    pveButton->setPos(xPos,yPos);
    connect(pveButton,SIGNAL(clicked()),this,SLOT(start()));
    scene->addItem(pveButton);

    Button *pvpButton = new Button(QString("PvP"),275,70);
    yPos = 280;
    pvpButton->setPos(xPos,yPos);
    connect(pvpButton,SIGNAL(clicked()),this,SLOT(close()));
    scene->addItem(pvpButton);

    Button *settingsButton = new Button(QString("Settings"),275,70);
    yPos = 360;
    settingsButton->setPos(xPos,yPos);
    connect(settingsButton,SIGNAL(clicked()),this,SLOT(close()));
    scene->addItem(settingsButton);

    Button *exitButton = new Button(QString("Exit"),275,70);
    yPos = 440;
    exitButton->setPos(xPos,yPos);
    connect(exitButton,SIGNAL(clicked()),this,SLOT(close()));
    scene->addItem(exitButton);
}


















