#ifndef GAME
#define GAME

#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsScene>
#include "tank.h"
#include "score.h"
#include "health.h"
#include "button.h"
#include "player.h"
#include "block.h"
#include "numupdown.h"
#include "textpanel.h"

QT_BEGIN_NAMESPACE
class QUdpSocket;
class QAction;
QT_END_NAMESPACE

class Game: public QGraphicsView{
    Q_OBJECT
public:
    Game(QWidget * parent=0);

    // меню
    Button **menuButtons;
    int numButtons; // сделано отдельно для обращения из класса button - кол-во кнопок
    bool pressed;  // была ли нажата кнопка

    TextPanel *text1;
    TextPanel *text2;
    numUpDown *num1;
    numUpDown *num2;
    numUpDown *res;
    Button *apply;
    Button *back;

    int veffects;
    int vmusic;

    int maxwidth;
    int maxheight;

    // сцена
    QGraphicsScene * scene;
    int dop;
    int xBlocks;
    int yBlocks;
    int spawns;
    QPoint **spawnPoints;

    // игрок
    Player * player;
    Score * score;
    Health * health;

    // просто танк
    int numtank;
    Tank * enmy;

    void moveToCenter();
    void change(QString name);

    // запреты на то, чтобы пользователь снял фокус с танка
    void mouseReleaseEvent(QMouseEvent * event);
    void focusOutEvent(QFocusEvent * event);
    void wheelEvent(QWheelEvent * event);
    void changeEvent(QEvent * event);
public slots:
    void applySettings();
    void switchButton(int n); // сменить кнопку на выбранную
    void menu(); // запустить меню
    void pve();
    void pvp();
    void settings();
    void processPendingDatagrams();
private:
    QUdpSocket *udpSocket;
};

#endif // GAME













