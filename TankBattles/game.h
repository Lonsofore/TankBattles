#ifndef GAME
#define GAME

#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsScene>
#include <QTcpSocket>
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
class QTcpSocket;
QT_END_NAMESPACE

class Game: public QGraphicsView
{
    Q_OBJECT
public:
    Game(QWidget * parent=0);

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
    int blockSize;
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

private slots:
    void SendData();

public slots:
    void switchButton(int n); // сменить кнопку на выбранную

    void menu(); // запустить меню
    void pve();  // пве

    void pvp();  // открывает меню выбора пвп
    void pvp1(); // присоединиться
    void pvp2(); // создать

    void settings();  // список настроек
    void pSettings(); // настройки игрока
    void mSettings(); // настройки игры
    void applySettings(); // применить настройки

    void gameMenu(); // внутриигровое меню
    void toGameMenu(); // в меню
    void back(); // вернуться в игру
    void gSettings(); // настройки игры из игры
    void toSettings(); //  в настройки
    void toMenu(); // вернуться в меню

    void processPendingDatagrams();
private:
    // кнопки для менюшек
    Button **btns;
    numUpDown **udBtns;

    // плашки для менюшек
    TextPanel *backgr;
    TextPanel *backgr1;

    // текстовые плашки
    TextPanel *text1;
    TextPanel *text2;

    // мультиплеерная ерунда
    QUdpSocket *udpSocket;
    QTcpSocket *tcpSocket;
    int usrid;
    bool isrecieving;
};

#endif // GAME













