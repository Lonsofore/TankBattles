#ifndef HEALTH
#define HEALTH

#include <QGraphicsItem>

class Health: public QGraphicsTextItem
{
public:
    Health(QGraphicsItem * parent = 0);
    void decrease(int v);
    int getHealth();
private:
    int health;
};

#endif // HEALTH

