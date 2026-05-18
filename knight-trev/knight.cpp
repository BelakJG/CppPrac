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

void Knight::printMoves() const
{
    if (prevMove)
    {
        prevMove->printMoves();
        std::cout << " -> ";
    }
    std::cout << start;
}