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
    Tank * enmy;

    void moveToCenter();
    void change(QString name);

    QImage setImageLightness(QImage img, int lightness);

    // запреты на то, чтобы пользователь снял фокус с танка
    void mouseReleaseEvent(QMouseEvent * event);
    void focusOutEvent(QFocusEvent * event);
    void wheelEvent(QWheelEvent * event);
    void changeEvent(QEvent * event);

private slots:
    void SendData();
    void readResponse();
    void waitForStart();
    void close();
    void closeEvent(QCloseEvent *);

public slots:
    void switchButton(int n); // сменить кнопку на выбранную

    void menu(); // запустить меню

    void pve();  // пве

    void pvp();  // открывает меню выбора пвп
    void pvp1(); // присоединиться
    void pvp2(); // создать
    void createServ();
    void pvpConnect(); // подключение
    void pvpLoad(QString filename); //Загрузка карты для MP

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

    // боты
    Bot **bots;

    // мультиплеер
    QUdpSocket *udpSocket;
    QTcpSocket *tcpSocket;
    int usrid; //ID игрока
    bool isrecieving, issending, inMP;
    QProcess *serv;

};

#endif // GAME













