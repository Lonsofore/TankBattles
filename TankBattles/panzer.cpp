#include "panzer.h"
#include <QPushButton>
#include "game.h"
#include <QMessageBox>

Panzer::Panzer()
{
    QMessageBox msgBox;
    msgBox.setText("YAY");
    msgBox.exec();
}

void Panzer::spawn()
{
    game* gm = new game();
    QPushButton *pb=new QPushButton(tr("NewButton"),gm);
    pb->setGeometry(20, 20, 80, 80);
    pb->show();
}
