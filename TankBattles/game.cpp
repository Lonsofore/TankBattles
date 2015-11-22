#include "game.h"
#include <QTimer>
#include <QGraphicsTextItem>
#include <QFont>
#include "enemy.h"
#include <QMediaPlayer>

Game::Game(QWidget *parent){
    // create the scene
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,800,600); // make the scene 800x600 instead of infinity by infinity (default)

    // make the newly created scene the scene to visualize (since Game is a QGraphicsView Widget,
    // it can be used to visualize scenes)
    setScene(scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(800,600);

    // create the player
    player = new Tank();

    // add the player to the scene
    scene->addItem(player);
    scene->addItem(player->head);

    // create the score/health
    score = new Score();
    scene->addItem(score);

/*
    health = new Health();
    health->setPos(player->x()+40,player->y()+50);
    scene->addItem(health);
*/

    // spawn enemies
    QTimer * timer = new QTimer();
    QObject::connect(timer,SIGNAL(timeout()),player,SLOT(spawn()));
    timer->start(2000);

    // ambient
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
