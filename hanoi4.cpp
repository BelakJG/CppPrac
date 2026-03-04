#include <iostream>
#include <cmath>
using namespace std;

int step = 1;

void hanoi3(int num_rings, string start, string dest, string aux) {
    if (num_rings == 1) {
        cout << step << ": Move ring from " << start << " to " << dest << endl;
        step += 1;
        return;
    }

    hanoi3(num_rings - 1, start, aux, dest);
    cout << step << ": Move ring from " << start << " to " << dest << endl;
    step += 1;
    hanoi3(num_rings - 1, aux, dest, start);
}

int find_k(int n) {
    int k = int(n - round(sqrt((2 * n) + 1)) + 1);
    return k;
}

void hanoi4(int num_rings, string start = "A", string dest = "D", string left = "B", string right = "C") {
    if (num_rings == 1) {
        cout << step << ": Move ring from " << start << " to " << dest << endl;
        step += 1;
        return;
    }

    int k = find_k(num_rings);
    hanoi4(k, start, left, right, dest);
    hanoi3(num_rings - k, start, dest, right);
    hanoi4(k, left, dest, right, start);
}