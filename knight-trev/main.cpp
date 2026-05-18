#include "knight.h"
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

int main()
{
    vector<Coords> visited;
    Coords shifts[8] = {
        {2, 1}, {1, 2}, {-1, 2}, {-2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}
    };
    Coords knightStart = {0, 0};
    Coords knightTarget = {7, 7};
    
    visited.push_back(knightStart);
    queue<Knight*> knightQueue;
    knightQueue.push(new Knight(knightStart, knightTarget));

    while (!knightQueue.empty() && !knightQueue.front()->isSolved()) {
        Knight *currentKnight = knightQueue.front();
        knightQueue.pop();
        Coords currentStart = currentKnight->getStart();
        for (Coords shift: shifts) {
            Coords newStart = currentStart + shift;
            if (newStart.valid() && find(visited.begin(), visited.end(), newStart) == visited.end()) {
                visited.push_back(newStart);
                knightQueue.push(new Knight(newStart, knightTarget, currentKnight));
            }
        }
    }

    knightQueue.front()->printMoves();
    cout << endl;

    while (!knightQueue.empty()) {
        delete knightQueue.front();
        knightQueue.pop();
    }
    return 0;
}