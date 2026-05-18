#ifndef KNIGHT_H
#define KNIGHT_H
#include <iostream>

struct Coords
{
    int x, y;

    bool operator==(const Coords &other) const
    {
        return (x == other.x && y == other.y);
    }

    Coords operator+(const Coords &other) const
    {
        return Coords{x + other.x, y + other.y};
    }

    friend std::ostream &operator<<(std::ostream &os, const Coords &c)
    {
        os << ("[X: " + std::to_string(c.x) + ", Y: " + std::to_string(c.y) + "]");
        return os;
    }

    bool valid() {
        return (x >= 0 && x < 8) && (y >= 0 && y < 8);
    }
};

class Knight
{
public:
    Knight(Coords start, Coords target, Knight *prevMove = nullptr);
    ~Knight();
    void printMoves();
    bool isSolved() { return solved; }
    Coords getStart() { return start; }
    Coords getTarget() { return target; }

private:
    Knight *prevMove;
    Coords start;
    Coords target;
    bool solved = false;
};

#endif