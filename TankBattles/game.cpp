#include "game.h"
#include <QTimer>
#include <QGraphicsTextItem>
#include <QFont>
#include "enemy.h"
#include <QMediaPlayer>

Game::Game(QWidget *parent){
    // создание сцены
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,800,600); //разрешение экрана

    // make the newly created scene the scene to visualize (since Game is a QGraphicsView Widget,
    // it can be used to visualize scenes)
    setScene(scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(800,600);

    // создание игрока
    player = new Tank();
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

    show();
}

void Game::mouseReleaseEvent(QMouseEvent *event)
{
    player->setFocus();
}
