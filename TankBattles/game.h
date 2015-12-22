#ifndef GAME
#define GAME

#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsScene>
#include <QTcpSocket>
#include <QProcess>

#include "tank.h"
#include "score.h"
#include "health.h"
#include "button.h"
#include "player.h"
#include "block.h"
#include "numupdown.h"
#include "textpanel.h"
#include "textbox.h"
#include "bot.h"

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

    bool darkMode; // test

    bool createBots;
    int nBots;
    QString fName;

    QString mapname;

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
    Tank * enmy[10];

    void moveToCenter();
    void change(QString name);

    QImage setImageLightness(QImage img, int lightness);

private slots:
    void SendData();
    void readResponse();

    void waitForStart();
    void beginGame();
    void close();
    void closeEvent(QCloseEvent *);

public slots:
    void switchButton(int n); // сменить кнопку на выбранную

    void menu(); // запустить меню

    void pve();  // загрузка пве
    void gpve(); // пве

    void pvp();  // открывает меню выбора пвп
    void pvp1(); // присоединиться
    void pvp2(); // создать
    void createServ();
    void pvpConnect(); // подключение
    void pvpLoading();
    void pvpLoad(); //Загрузка карты для MP
    int fileCheck(QString filen); //Проверка карты на соответствие стандартам

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
    // массивы элементов менюшек
    Button **btns;
    numUpDown **udBtns;
    TextBox **tBoxes;

    // плашки для менюшек
    TextPanel *backgr;
    TextPanel *backgr1;

    // текстовые плашки
    TextPanel *text1;
    TextPanel *text2;
    TextPanel *text3;

    // боты
    Bot **bots;

    // мультиплеер
    bool isHost;
    int numPlayers;

    QGraphicsTextItem *waitText;

    QUdpSocket *udpSocket;
    QTcpSocket *tcpSocket;
    int usrid; //ID игрока
    int pCnt; //Кол-во игроков
    QString map;
    bool isrecieving, issending, inMP, gameStarted;
    QProcess *serv;
    bool isSpwnd, isExititng;

};

#endif // GAME













