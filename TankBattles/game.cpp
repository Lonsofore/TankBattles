#include "game.h"
#include <QTimer>
#include <QGraphicsTextItem>
#include <QFont>
#include "enemy.h"
#include <QMediaPlayer>
#include "button.h"
#include "player.h"

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

    show();
}

void Game::mouseReleaseEvent(QMouseEvent *event)
{
    if (started)
        player->setFocus();
}
/*
void Game::keyPressEvent(QKeyEvent *event)
{
    if (started == false)
    {
        switch ((uint) event->key())
        {
            case 16777235: // Up
                if (curButton > 0)
                    curButton--;
                else
                    curButton = 3;
                switchButton();
            break;

            case 16777237: // Down
                if (curButton < 3)
                    curButton++;
                else
                    curButton = 0;
                switchButton();
            break;

            case 16777220: // Enter
                menuButtons[curButton]->clicked();
            break;
        }
    }
}
*/
void Game::switchButton() // смена кнопки
{
    for (int i=0; i<numButton; i++)
        menuButtons[i]->deselect();

    menuButtons[curButton]->select();
}

void Game::start()
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
    int yPos = 200;
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
    connect(menuButtons[0],SIGNAL(clicked()),this,SLOT(start()));
    connect(menuButtons[3],SIGNAL(clicked()),this,SLOT(close()));

    //menuButtons[curButton]->select();
    switchButton();
}


















