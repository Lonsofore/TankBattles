#ifndef GAME_H
#define GAME_H

#include "tank.h"
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
    void keyPressEvent(QKeyEvent* event);
    void spawntank();
private:
    Ui::game *ui;
};

#endif // GAME_H
