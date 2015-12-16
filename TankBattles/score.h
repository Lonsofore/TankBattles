#ifndef SCORE
#define SCORE

#include <QGraphicsItem>

class Score: public QGraphicsTextItem
{
public:
    Score(QGraphicsItem * parent = 0);
    void increase();
    int getScore();
private:
    int score; 
};

#endif // SCORE

