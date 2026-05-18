#include "knight.h"

Knight::Knight(Coords start, Coords target, Knight *prevMove)
{
    this->start = start;
    this->target = target;
    this->prevMove = prevMove;

    if (start == target)
    {
        solved = true;
    }
}

Knight::~Knight() {
    std::cout << "Knight destroyed" << std::endl;
}

void Knight::printMoves()
{
    if (prevMove)
    {
        prevMove->printMoves();
        std::cout << " -> ";
    }
    std::cout << start;
}