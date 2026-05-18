#include "knight.h"
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

int main()
{
    int visited[8][8] = {};
    vector<Knight *> savedKnights;
    Coords shifts[8] = {
        {2, 1}, {1, 2}, {-1, 2}, {-2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}};
    Coords knightStart = {2, 2};
    Coords knightTarget = {7, 7};

    visited[knightStart.y][knightStart.x] = 1;
    queue<Knight *> knightQueue;
    Knight *firstKnight = new Knight(knightStart, knightTarget);
    knightQueue.push(firstKnight);

    while (!knightQueue.empty() && !knightQueue.front()->isSolved())
    {
        Knight *currentKnight = knightQueue.front();
        knightQueue.pop();
        Coords currentStart = currentKnight->getStart();
        bool save = false;
        for (Coords shift : shifts)
        {
            Coords newStart = currentStart + shift;
            if (newStart.valid() && visited[newStart.y][newStart.x] != 1)
            {
                visited[newStart.y][newStart.x] = 1;
                Knight *newKnight = new Knight(newStart, knightTarget, currentKnight);
                knightQueue.push(newKnight);
                save = true;
            }
        }
        if (!save)
        {
            delete currentKnight;
        }
        else
        {
            savedKnights.push_back(currentKnight);
        }
    }

    if (!knightQueue.empty() && knightQueue.front()->isSolved())
    {
        knightQueue.front()->printMoves();
        cout << endl;
    }
    else
    {
        cout << "Solution not found!" << endl;
    }

    for (Knight *savedKnight : savedKnights)
    {
        delete savedKnight;
    }

    return 0;
}