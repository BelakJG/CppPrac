#include <iostream>
using namespace std;

int step = 1;

void hanoi3(int num_rings, string start = "A", string dest = "C", string aux = "B") {
    if (num_rings == 1) {
        cout << step << ": Move ring " << start << " to " << dest << endl;
        step += 1;
        return;
    }

    hanoi3(num_rings - 1, start, aux, dest);
    cout << step << ": Move ring " << start << " to " << dest << endl;
    step += 1;
    hanoi3(num_rings - 1, aux, dest, start);
}