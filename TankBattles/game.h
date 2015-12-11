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

    // кнопки для менюшек
    Button **btns;
    numUpDown **udBtns;

    // громкость музыки и эффектов
    int vmusic;
    int veffects;

    // максимальное допустимое разрешение экрана (для обрезки пустых мест карты)
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
    void switchButton(int n); // сменить кнопку на выбранную

    void menu(); // запустить меню
    void pve();

    void pvp();
    void pvp1();
    void pvp2();

    void settings();
    void pSettings();
    void mSettings();
    void applySettings();

    void processPendingDatagrams();
private:
    QUdpSocket *udpSocket;
};

#endif // GAME













