#ifndef PREVIEW_H
#define PREVIEW_H

#include <QMainWindow>
#include "QGraphicsScene"

namespace Ui {
class preview;
}

class preview : public QMainWindow
{
    Q_OBJECT

public:
    explicit preview(QWidget *parent = 0);
    ~preview();
public slots:

    void RecieveValue(int itm);

    void RecieveSize(int x, int y);

signals:

    void GetData(int row, int column);

    void GetSize();

    void GetItem(int row, int column);

private slots:

    void on_preview_windowTitleChanged(const QString &title);

private:
    Ui::preview *ui;
    void closeEvent(QCloseEvent *);
    bool eventFilter(QObject *, QEvent *);
    void draw();
    int citm;
    int xsize;
    int ysize;
};

#endif // PREVIEW_H
