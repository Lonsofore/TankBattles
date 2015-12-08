#include "block.h"

Block::Block()
{

}

Block::Block(int n)
{
    num = n;

    if (num == 3)
        setZValue(5);
    else
        setZValue(1);
}

