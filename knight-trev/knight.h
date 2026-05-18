#ifndef KNIGHT_H
#define KNIGHT_H
#include <iostream>
#include <string>

struct Coords
{
    int x, y;

    Coords() : x(0), y(0) {}

    Coords(int x, int y) : x(x), y(y) {}

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

    bool valid() const
    {
        return (x >= 0 && x < board_size) && (y >= 0 && y < board_size);
    }

private:
    int board_size = 8;
};

class Knight
{
public:
    Knight(Coords start, Coords target, Knight *prevMove = nullptr);
    ~Knight() = default;
    void printMoves() const;
    bool isSolved() const { return solved; }
    Coords getStart() const { return start; }
    Coords getTarget() const { return target; }

private:
    Knight *prevMove;
    Coords start;
    Coords target;
    bool solved = false;
};

#endif