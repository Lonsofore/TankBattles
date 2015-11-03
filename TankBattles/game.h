#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QLabel>
#include <QString>

namespace Ui {
class game;
}

class game : public QWidget
{
    Q_OBJECT

public:
    explicit game(QWidget *parent = 0);
    ~game();
    void showEvent(QShowEvent *);
    void keyPressEvent(QKeyEvent* event);
    void rotateTank();

    QString img;
    QLabel *pic;
    int x,y,dir,speed;

private:
    Ui::game *ui;
};

#endif // GAME_H
